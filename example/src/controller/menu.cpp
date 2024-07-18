#include "controller/controller_states.h"
#include "controller/menu.h"
#include "app/input.h"

#include <ldtools/ttf_manager.h>
#include <tools/json.h>
#include <tools/file_utils.h>
#include <tools/i8n.h>
#include <ldv/ttf_representation.h>

#include <iostream>

using namespace controller;

menu::menu(
	app::service_provider& _sp
):
	sp{_sp},
	env{_sp.get_env()},
	logger{_sp.get_logger()}
{

	//Chicken and egg... can we parse first and map later so we can define
	//these stuffs??
	view.map_font(
		"main_menu_font",
		sp.get_ttf_manager().get(
			"menu_font",
			16
		)
	);

	const std::string layout_path=env.build_app_path("resources/layout/main_menu.json");
	auto document=tools::parse_json_string(tools::dump_file(layout_path));
	view.parse(document["main_menu"]);

	auto set_text=[this](const std::string& _id, const std::string& _key) {

		static_cast<ldv::ttf_representation*>(view.get_by_id(_id))->set_text(
			sp.get_localization().get(_key)
		);
	};

	set_text("menu_start", "main_menu-start");
	set_text("menu_continue", "main_menu-continue");
	set_text("menu_quit", "main_menu-quit");
	set_text("menu_skill_easy", "main_menu-skill_easy");
	set_text("menu_skill_normal", "main_menu-skill_normal");
	set_text("menu_skill_hard", "main_menu-skill_hard");

	enter_main();
}

void menu::awake(
	dfw::input& /*input*/
) {

	refresh();
}

void menu::slumber(
	dfw::input& /*input*/
) {

}

void menu::loop(
	dfw::input& _input,
	const dfw::loop_iteration_data& /*_lid*/
) {

	if(_input().is_exit_signal()) {
		set_leave(true);
		return;
	}

	//TODO: Or a specific SDL key for each of these inputs.
	if(_input.is_input_down(app::input::escape)) {

		back();
		return;
	}

	if(_input.is_input_down(app::input::down)) {

		next();
		return;
	}

	if(_input.is_input_down(app::input::up)) {

		prev();
		return;
	}

	//TODO: Jump, really? This is a placeholder.
	if(_input.is_input_down(app::input::jump)) {

		select();
		return;
	}
}

void menu::draw(
	ldv::screen& _screen,
	int /*_fps*/
) {

	_screen.clear({0, 0, 0, 0});
	view.draw(_screen);
}

void menu::next() {

	int * curoption=nullptr;
	int max=0;
	switch(curlevel) {
		case levels::main: 
			curoption=&main_option; 
			max=main_option_exit;
		break;
		case levels::skill: 
			curoption=&skill_option; 
			max=skill_option_hard;
		break;
	}

	if(*curoption == max) {

		return;
	}

	play_sound(app::snd_switch);
	++(*curoption);
	refresh();
}

void menu::prev() {

	int * curoption=nullptr;
	switch(curlevel) {
		case levels::main: 
			curoption=&main_option; 
		break;
		case levels::skill: 
			curoption=&skill_option; 
		break;
	}

	if(*curoption == 0) {

		return;
	}

	play_sound(app::snd_switch);
	--(*curoption);
	refresh();
}

void menu::select() {

	switch(curlevel) {

		case levels::main:

			switch(main_option) {
				case main_option_choose_skill:

					curlevel=levels::skill;
					enter_new_game();
					return;
				case main_option_continue:

					if(game_can_continue) {

						continue_game=true;
						push_state(state_main);
						return;
					}

					//TODO: We should... I don't know, reserve a channel for this. We
					//can hit the button until the cows come home xD!
					play_sound(app::snd_defeat);
					return;

				case main_option_exit:
					set_leave(true);
					return;
			}

			return;

		case levels::skill:

			//Do something extra, next time we are here we will be on "continue".
			enter_main();
			curlevel=levels::main;
			main_option=main_option_continue;
			push_state(state_main);
			continue_game=false;
			return;
	}
}

void menu::back() {

	switch(curlevel) {

		case levels::main:

			set_leave(true);
			return;

		case levels::skill:

			curlevel=levels::main;
			enter_main();
			return;
	}
}

void menu::enter_main() {

	auto toggle=[this](const std::string& _id, bool _value) {

		static_cast<ldv::representation*>(view.get_by_id(_id))->set_visible(_value);
	};

	toggle("menu_start", true);
	toggle("menu_continue", true);
	toggle("menu_quit", true);
	toggle("menu_skill_easy", false);
	toggle("menu_skill_normal", false);
	toggle("menu_skill_hard", false);

	refresh();
}

void menu::enter_new_game() {

	auto toggle=[this](const std::string& _id, bool _value) {

		static_cast<ldv::representation*>(view.get_by_id(_id))->set_visible(_value);
	};

	toggle("menu_start", true);
	toggle("menu_continue", false);
	toggle("menu_quit", false);
	toggle("menu_skill_easy", true);
	toggle("menu_skill_normal", true);
	toggle("menu_skill_hard", true);

	refresh();
}

/**
 * maps local skill value to app skill value.
 */
int menu::get_selected_skill() const {

	switch(skill_option) {
		case skill_option_easy: return app::skill_easy;
		case skill_option_normal: return app::skill_normal;
		case skill_option_hard: return app::skill_hard;
		default:
			return app::skill_normal;
	}
}

/**
 * shows the currently selected option.
 */
void menu::refresh() {

	auto toggle=[this](const std::string& _id, bool _value) {

		int alpha=_value ? 255 : 128;
		static_cast<ldv::representation*>(view.get_by_id(_id))->set_alpha(alpha);
	};

	toggle("menu_start", false);
	toggle("menu_continue", false);
	toggle("menu_quit", false);
	toggle("menu_skill_easy", false);
	toggle("menu_skill_normal", false);
	toggle("menu_skill_hard", false);

	switch(curlevel) {

		case levels::main:

			switch(main_option) {

				case main_option_choose_skill:

					toggle("menu_start", true);
					return;

				case main_option_continue:

					toggle("menu_continue", true);
					return;
				case main_option_exit:

					toggle("menu_quit", true);
					return;

				default:
					return;
			}

		case levels::skill:

			toggle("menu_start", true);

			switch(skill_option) {

				case skill_option_easy:
					toggle("menu_skill_easy", true);
					return;
				case skill_option_normal:
					toggle("menu_skill_normal", true);
					return;
				case skill_option_hard:
					toggle("menu_skill_hard", true);
					return;
				default:
					return;
			}
	}
}

//TODO: Repeated :/. This appears on main too.
void menu::play_sound(
	int _index
) {

	lda::sound_struct snd{
		sp.get_audio_resource_manager().get_sound(_index)
	};

	sp.get_audio().play_sound(snd);
}
