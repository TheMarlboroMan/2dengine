#include "controller/options.h"
#include "app/input.h"
#include "app/env.h"
#include <ldtools/ttf_manager.h>
#include <tools/file_utils.h>
#include <tools/json.h>
#include <rapidjson/document.h>
#include <map>

using namespace controller;

options::options(
	app::service_provider& _sp
):
	sp{_sp},
	logger{sp.get_logger()},
	config{sp.get_config()},
	i8n{sp.get_localization()},
	audio_c{sp.get_audio_controller()},
	screen{sp.get_screen()},
	enter_timeout{0.5, 0., false}
{
	//Prepare the view...
	view.map_font(
		"main_menu_font",
		sp.get_ttf_manager().get(
			"menu_font",
			8
		)
	);

	const auto& env=sp.get_env();

	const std::string layout_path=env.build_app_path("resources/layout/views.json");
	auto document=tools::parse_json_string(tools::dump_file(layout_path));
	view.parse(document["options"]);

	std::map<std::string, std::string> lan_map={
		{"display_title", "options-display"},
		{"sound_title", "options-sound"},
		{"music_title", "options-music"},
		{"back_title", "options-back"}
	};

	for(const auto& pair : lan_map) {

		view.set_text(pair.first, i8n.get(pair.second));
	}
}

void options::awake(
	dfw::input&
) {

	index=0;
	consecutive_press_tics=0;
	refresh_index();
	update_values();
	enter_timeout.restart();
}

void options::slumber(
	dfw::input& 
) {

	lm::log(logger).info()<<"will save new values to config file"<<std::endl;
	config.save();
}

void options::loop(
	dfw::input& _input,
	const dfw::loop_iteration_data& _lid
) {

	enter_timeout.tic(_lid.delta);

	if(_input().is_exit_signal()) {
		set_leave(true);
		return;
	}

	if(_input().is_key_down(SDL_SCANCODE_ESCAPE)) { 

		pop_state();
	}

	//There are two sets of input checks here: one is basic keyboard for
	//navigation and the other is game-based. We try the generic first and
	//if no input is detected we try the game part. Just as in the menu,
	//but with the addition of left/right.

	enum class menu_input {
		up, down, select_less, select_more, none
	} choice=menu_input::none;

	if(_input().is_key_down(SDL_SCANCODE_DOWN)) {

		choice=menu_input::down;
	}
	else if(_input().is_key_down(SDL_SCANCODE_UP)) {

		choice=menu_input::up;
	}
	else if(
		_input().is_key_down(SDL_SCANCODE_SPACE)
		|| _input().is_key_pressed(SDL_SCANCODE_RETURN)
		|| _input().is_key_pressed(SDL_SCANCODE_RIGHT)
	) {

		choice=menu_input::select_more;
	}
	else if(_input().is_key_pressed(SDL_SCANCODE_LEFT)) {

		choice=menu_input::select_less;
	}

	//Attempt to detect input from game configuration.
	if(choice==menu_input::none) {

		if(_input.is_input_down(app::input::down)) {

			choice=menu_input::down;
		}
		else if(_input.is_input_down(app::input::up)) {

			choice=menu_input::up;
		}
		else if(
			_input.is_input_pressed(app::input::jump)
			|| _input.is_input_pressed(app::input::right)
		) {

			choice=menu_input::select_more;
		}
		else if(_input.is_input_pressed(app::input::left)) {

			choice=menu_input::select_less;
		}
	}

	//Do not accept a "select" input in the first seconds of the controller.

	switch(choice) {

		case menu_input::up:
			index==0
				? index=3
				: --index;
			refresh_index();
			return;

		case menu_input::down:
			index==3
				? index=0
				: ++index;
			refresh_index();
			return;

		case menu_input::select_more:

			if(!enter_timeout.is_finished()) {

				return;
			}

			//TODO: This would end up overflowing.
			consecutive_press_tics >= 0
				? ++consecutive_press_tics
				: consecutive_press_tics=1;

			select_option(1);
			return;

		case menu_input::select_less:

			if(!enter_timeout.is_finished()) {

				return;
			}

			//TODO: This would end up underflowing.
			consecutive_press_tics <= 0
				? --consecutive_press_tics
				: consecutive_press_tics=-1;

			select_option(-1);
			return;

		case menu_input::none:

			consecutive_press_tics=0;
			return;
	}
}

void options::draw(
	ldv::screen& _screen,
	int /*_fps*/
) {

	view.draw(_screen);
}

void options::refresh_index() {

	struct pair {
		std::string title, value;
	};

	std::vector<pair> view_map={
		{"display_title", "display_value"},
		{"sound_title", "sound_value"},
		{"music_title", "music_value"},
		{"back_title", ""}
	};

	std::size_t i=0;
	for(const auto& pair : view_map) {

		int alpha=i==index ? 255 : 128;
		i++;

		view.set_alpha(pair.title, alpha);
		if(pair.value.size()) {

			view.set_alpha(pair.value, alpha);
		}
	}
}

void options::select_option(
	int _direction
) {

	switch(index) {

		case option_display:

			select_display();
			return;

		case option_sound:

			select_sound(_direction);
			return;

		case option_music:

			select_music(_direction);
			return;

		case option_back:
			pop_state();
			return;
	}
}

void options::select_display() {

	bool new_value=!config.bool_from_path("video:fullscreen");
	screen.set_fullscreen(new_value);
	config.set("video:fullscreen", new_value);
	update_values();
}

void options::select_sound(
	int _direction
) {

	int current=config.get_audio_volume();
	
	//Cancel movement when pressing before N tics.
	if(abs(consecutive_press_tics) > 1 && abs(consecutive_press_tics) < 30) {

		return;
	}

	current+=_direction;
	if(current < 0) {

		current=0;
	}
	else if(current > 127) {

		current=127;
	}

	config.set_audio_volume(current);
	audio_c.set_main_sound_volume(current);
	update_values();
}

void options::select_music(
	int _direction
) {

	int current=config.get_music_volume();

	//Cancel movement when pressing before N tics.
	if(abs(consecutive_press_tics) > 1 && abs(consecutive_press_tics) < 30) {

		return;
	}

	current+=_direction;
	if(current < 0) {

		current=0;
	}
	else if(current > 127) {

		current=127;
	}

	config.set_music_volume(current);
	audio_c.set_main_music_volume(current);
	update_values();
}

void options::update_values() {
	
	view.set_text(
		"display_value", 
		config.bool_from_path("video:fullscreen")
			? i8n.get("options-display_fullscreen")
			: i8n.get("options-display_windowed")
	);

	int audio_vol=volume_to_gui(config.get_audio_volume());
	view.set_text(
		"sound_value", 
		std::to_string(audio_vol)
	);

	int music_vol=volume_to_gui(config.get_music_volume());
	view.set_text(
		"music_value", 
		std::to_string(music_vol)
	);
}

int options::volume_to_gui(
	int _volume
) const {

	//Convert a lib volume (0-127 to a 0-100 range).
	return (_volume * 100)  / 127;
}

int options::gui_to_volume(
	int _value
) const {

	//Convert a 0-100 range to lib (0 - 127).
	return (_value * 127) / 100;
}

