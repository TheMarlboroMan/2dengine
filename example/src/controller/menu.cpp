#include "controller/controller_states.h"
#include "controller/menu.h"
#include "app/input.h"
#include "app/map_attribute_loader.h"

#include <ldtools/ttf_manager.h>
#include <tools/json.h>
#include <tools/file_utils.h>
#include <tools/i8n.h>
#include <tools/time.h>
#include <tools/number_utils.h>

#include <ldv/ttf_representation.h>

#include <d2d/storage/map_loader.h>

#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>

using namespace controller;

menu::menu(
	app::service_provider& _sp
):
	sp{_sp},
	env{_sp.get_env()},
	logger{_sp.get_logger()},
	sound_player{_sp.get_sound_player()},
	savegame_manager{env},
	automap_interface{sp.get_automap()}
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

	view.map_font(
		"main_menu_small_font",
		sp.get_ttf_manager().get(
			"menu_font",
			8
		)
	);

	const std::string layout_path=env.build_app_path("resources/layout/views.json");
	auto document=tools::parse_json_string(tools::dump_file(layout_path));
	view.parse(document["main_menu"]);


	const auto& i8n=sp.get_localization();

	view.set_text("menu_start", i8n.get("main_menu-start"));
	view.set_text("menu_continue", i8n.get("main_menu-continue"));
	view.set_text("menu_controls", i8n.get("main_menu-controls"));
	view.set_text("menu_options", i8n.get("main_menu-options"));
	view.set_text("menu_quit", i8n.get("main_menu-quit"));
	view.set_text("menu_skill_easy", i8n.get("main_menu-skill_easy"));
	view.set_text("menu_skill_normal", i8n.get("main_menu-skill_normal"));
	view.set_text("menu_skill_hard", i8n.get("main_menu-skill_hard"));
	view.set_text("menu_savegame_delete_hint", i8n.get("main_menu-delete_slot"));
	view.set_text("menu_savegame_delete_confirm", i8n.get("main_menu-delete_slot_confirm"));

	enter_main();
}

void menu::awake(
	dfw::input& /*input*/
) {

	savegame_manager.load();
	refresh();
	refresh_save_slots();
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

	if(_input.is_input_down(app::input::escape)
		|| _input().is_key_down(SDL_SCANCODE_ESCAPE)
	) {

		back();
		return;
	}

	//TODO: This is now repeated.
	//There are two sets of input checks here: one is basic keyboard for
	//navigation and the other is game-based. We try the generic first and
	//if no input is detected we try the game part.

	enum class menu_input {
		up, down, select, none
	} choice=menu_input::none;

	if(_input().is_key_down(SDL_SCANCODE_DOWN)) {

		choice=menu_input::down;
	}
	else if(_input().is_key_down(SDL_SCANCODE_UP)) {

		choice=menu_input::up;
	}
	else if(
		_input().is_key_down(SDL_SCANCODE_SPACE)
		|| _input().is_key_down(SDL_SCANCODE_RETURN)
	) {

		choice=menu_input::select;
	}

	//Attempt to detect input from game configuration.
	if(choice==menu_input::none) {

		if(_input.is_input_down(app::input::down)) {

			choice=menu_input::down;
		}
		else if(_input.is_input_down(app::input::up)) {

			choice=menu_input::up;
		}
		else if(_input.is_input_down(app::input::jump)) {

			choice=menu_input::select;
		}
	}

	switch(choice) {

		case menu_input::up:
			prev();
			return;
		case menu_input::down:
			next();
			return;
		case menu_input::select:
			select();
			return;
		case menu_input::none:
			break;
	}

	//And this is so specific we can just use the game controls.
	if(_input.is_input_down(app::input::pause)) {

		if(levels::slot==curlevel) {

			remove();
		}
		else if(levels::confirm_delete==curlevel) {

			confirm_delete();
			return;
		}
	}
}

void menu::draw(
	ldv::screen& _screen,
	int /*_fps*/
) {

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
		case levels::slot:
			curoption=&slot_option; 
			max=slot_three;
		break;
		case levels::skill: 
			curoption=&skill_option; 
			max=skill_option_hard;
		break;
		case levels::confirm_delete:
			exit_confirm_delete();
			return;
	}

	if(*curoption == max) {

		return;
	}

	sound_player.play_once(app::snd_switch);
	++(*curoption);
	refresh();
}

void menu::prev() {

	int * curoption=nullptr;
	switch(curlevel) {
		case levels::main: 
			curoption=&main_option; 
		break;
		case levels::slot: 
			curoption=&slot_option; 
		break;
		case levels::skill: 
			curoption=&skill_option; 
		break;
		case levels::confirm_delete:
			exit_confirm_delete();
			return;
	}

	if(*curoption == 0) {

		return;
	}

	sound_player.play_once(app::snd_switch);
	--(*curoption);
	refresh();
}

void menu::select() {

	switch(curlevel) {

		case levels::main:

			switch(main_option) {
				case main_option_choose_slot:

					enter_slot_select();
					return;

				case main_option_continue:

					attempt_to_continue();
					return;

				case main_option_controls:

					enter_controls();
					return;

				case main_option_options:

					enter_options();
					return;

				case main_option_exit:
					set_leave(true);
					return;
			}

			return;

		case levels::slot:

			choose_slot();
			return;

		case levels::skill:

			choose_skill();
			return;

		case levels::confirm_delete:
			exit_confirm_delete();
			return;
	}
}

void menu::back() {

	switch(curlevel) {

		case levels::main:

			set_leave(true);
			return;

		case levels::slot:

			enter_main();
			return;

		case levels::skill:

			curlevel=levels::slot;
			enter_slot_select();
			return;

		case levels::confirm_delete:
			exit_confirm_delete();
			return;
	}
}

void menu::enter_main() {

	curlevel=levels::main;
	ready_main();
}

void menu::enter_controls() {

	push_state(state_controls);
}

void menu::enter_options() {

	push_state(state_options);
}

void menu::enter_slot_select() {

	curlevel=levels::slot;
	ready_slot_select();
}

void menu::enter_skill_select() {

	curlevel=levels::skill;
	ready_skill_select();
}

void menu::ready_main() {

	set_visible_main(true);
	set_visible_skill_select(false);
	set_visible_slot_select(false);

	refresh();
}


void menu::ready_slot_select() {

	set_visible_slot_select(true);
	set_visible_main(false);
	view.set_visible("menu_start", true); //TODO: actually,. we could have another text...

	refresh();
}

void menu::set_visible_main(
	bool _val
) {

	for(const auto str : {
		"menu_start",
		"menu_continue",
		"menu_controls",
		"menu_options",
		"menu_quit",
	}) {

		view.set_visible(str, _val);
	}
}

void menu::set_visible_slot_select(
	bool _val
) {

	for(const auto str : {
		"menu_savegame_slot_1", 
		"menu_savegame_slot_2", 
		"menu_savegame_slot_3",
		"menu_savegame_description",
		"menu_savegame_delete_hint"
	}) {

		view.set_visible(str, _val);
	}
}

void menu::set_visible_skill_select(
	bool _val
) {

	for(const auto str : {
		"menu_skill_easy", 
		"menu_skill_normal",
		"menu_skill_hard"
	}) {

		view.set_visible(str, _val);
	}
}

void menu::ready_skill_select() {

	set_visible_skill_select(true);
	set_visible_main(false);
	set_visible_slot_select(false);

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
			throw std::runtime_error("bad skill");
	}
}

const std::string& menu::get_slot_filename() const {

	switch(slot_option) {
		case slot_one: return savegame_manager.get(0).filename;
		case slot_two: return savegame_manager.get(1).filename;
		case slot_three: return savegame_manager.get(2).filename;
		default:
			throw std::runtime_error("bad slot");
	}
}

/**
 * shows the currently selected option.
 */
void menu::refresh() {

	auto toggle=[this](const std::string& _id, bool _value) {

		int alpha=_value ? 255 : 128;
		view.set_alpha(_id, alpha);
	};

	for(auto str : {
		"menu_skill_easy",
		"menu_skill_normal",
		"menu_skill_hard",
		"menu_start",
		"menu_continue",
		"menu_controls",
		"menu_options",
		"menu_quit",
		"menu_savegame_slot_1", 
		"menu_savegame_slot_2", 
		"menu_savegame_slot_3",
		"menu_savegame_description",
		"menu_savegame_delete_hint"
	}) {

		toggle(str, false);
	}

	switch(curlevel) {

		case levels::main:

			switch(main_option) {

				case main_option_choose_slot:

					toggle("menu_start", true);
					return;

				case main_option_continue:

					toggle("menu_continue", true);
					return;

				case main_option_controls:

					toggle("menu_controls", true);
					return;

				case main_option_options:

					toggle("menu_options", true);
					return;

				case main_option_exit:

					toggle("menu_quit", true);
					return;

				default:
					return;
			}

		case levels::slot:

			toggle("menu_start", true);
			set_savegame_description(slot_option);

			switch(slot_option) {

				case slot_one:
					toggle("menu_savegame_slot_1", true);
					return;
				case slot_two:
					toggle("menu_savegame_slot_2", true);
					return;
				case slot_three:
					toggle("menu_savegame_slot_3", true);
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

		case levels::confirm_delete:
			return;
	}
}

/**
 * attempt to continue an ongoing game.
 */
void menu::attempt_to_continue() {

	if(game_can_continue) {

		action_signal=signals::continue_game;
		push_state(state_main);
		return;
	}

	if(!sound_guard.playing) {

		sound_player.play_once_then(app::snd_defeat, sound_guard);
	}
}

void menu::choose_slot() {

	const auto& slot=savegame_manager.get(slot_option);

	//This is "load a game".
	if(!slot.new_game) {

		load_game();
		return;
	}

	enter_skill_select();
}

void menu::choose_skill() {

	//Do something extra, next time we are here we will be on "continue".
	enter_main();
	main_option=main_option_continue;
	push_state(state_main);
	action_signal=signals::new_game;
}

void menu::refresh_save_slots() {

	const auto& slots=savegame_manager.get_slots();

	std::map<int, std::string> slot_to_id={
		{0, "menu_savegame_slot_1"},
		{1, "menu_savegame_slot_2"},
		{2, "menu_savegame_slot_3"}
	};

	for(const auto& pair: slot_to_id) {

		std::stringstream ss;
		ss<<(pair.first+1)<<": ";

		if(slots[pair.first].new_game) {

			ss<<sp.get_localization().get("main_menu-new_game");
		}
		else {

			ss<<sp.get_localization().get("main_menu-continue_game");
			//TODO: COllectibles, time, skill, etc...
		}

		view.set_text(pair.second, ss.str());
	}

}

void menu::load_game() {

	enter_main();
	main_option=main_option_continue;
	push_state(state_main);
	action_signal=signals::load_game;
}

void menu::set_savegame_description(
	int _slot
) {

	const auto& slot=savegame_manager.get(_slot);
	view.set_visible("menu_savegame_description", !slot.new_game);
	view.set_visible("menu_savegame_delete_hint", !slot.new_game);

	if(slot.new_game) {

		return;
	}

	const auto& i8n=sp.get_localization();
	std::stringstream ss;

	switch(slot.skill_setting) {

		case app::skill_easy: ss<<i8n.get("main_menu-skill_easy"); break;
		case app::skill_normal: ss<<i8n.get("main_menu-skill_normal"); break;
		case app::skill_hard: ss<<i8n.get("main_menu-skill_hard"); break;
	}

	//we always draw the amount of stuff collected.
	
	//TODO: Count the real items!!...
	const int total_stuff=100;
	int stuff_collected=tools::percent(slot.collectibles, total_stuff);

	ss<<", "<<stuff_collected<<"%";

	//Also, on hard and normal there is a time limit!
	if(slot.skill_setting != app::skill_easy) {

		tools::time t;
		auto ts=t.seconds_to_timedata(slot.elapsed_seconds);
		ss<<", "<<t.time_to_string(ts.hours, ts.minutes, ts.seconds);
	}

	int area_id=automap_interface.area_id_from_map(slot.levelname);
	const auto& area=automap_interface.get(area_id);
	ss<<"\n\n  "<<sp.get_localization().get(area.localization_key);

	view.set_text("menu_savegame_description", ss.str());
}

void menu::remove() {

	if(levels::slot!=curlevel) {

		return;
	}

	//Cannot remove a new-game file.
	const auto& slot=savegame_manager.get(slot_option);
	if(slot.new_game) {

		return;
	}

	//Enter the confirm delete...
	view.set_visible("menu_savegame_delete_hint", false);
	view.set_visible("menu_savegame_delete_confirm", true);
	curlevel=levels::confirm_delete;
}

void menu::confirm_delete() {

	savegame_manager.erase(slot_option);
	refresh_save_slots();
	exit_confirm_delete();
}

void menu::exit_confirm_delete() {

	enter_slot_select();
	view.set_visible("menu_savegame_delete_confirm", false);
}
