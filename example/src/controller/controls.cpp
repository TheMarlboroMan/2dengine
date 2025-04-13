#include "controller/controls.h"
#include "app/input.h"
#include "app/env.h"
#include <ldtools/ttf_manager.h>
#include <tools/file_utils.h>
#include <tools/json.h>
#include <rapidjson/document.h>
#include <ldi/filter.h>

using namespace controller;

controls::controls(
	app::service_provider& _sp
):
	sp{_sp},
	logger{sp.get_logger()},
	config{sp.get_config()},
	i8n{sp.get_localization()}
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
	view.parse(document["controls"]);

	view.set_text("enter_input_for", i8n.get("controls-legend"));
	view.set_text("press_escape_to_cancel", i8n.get("controls-cancel"));

	//The way this works, we have a list of structures that relate
	//a config input entry with a i8n entry and an input entry itself.
	//These are filled up sequentially when entering this controller. If
	//all are filled up we update the input configuration.

	using dfw::input_description;
	
	input_description def{input_description::types::none, 0, 0};
	
	entries.push_back({app::up, "controls-up", "input:up", def});
	entries.push_back({app::down, "controls-down", "input:down", def});
	entries.push_back({app::left, "controls-left", "input:left", def});
	entries.push_back({app::right, "controls-right", "input:right", def});
	entries.push_back({app::jump, "controls-jump", "input:jump", def});
	entries.push_back({app::pause, "controls-pause", "input:pause", def});
}

void controls::awake(
	dfw::input&
) {

	//reset the question array, inputs, flags...
	index=0;
	dfw::input_description def{dfw::input_description::types::none, 0, 0};
	for(auto& node : entries) {

		node.input=def;
	}

	view.set_text("target_input", i8n.get(entries.at(index).i8n_key));
}

void controls::slumber(
	dfw::input& 
) {

}

void controls::loop(
	dfw::input& _input,
	const dfw::loop_iteration_data& /*_lid*/
) {

	if(_input().is_exit_signal()) {
		set_leave(true);
		return;
	}

	if(_input().is_key_down(SDL_SCANCODE_ESCAPE)) {

		lm::log(logger).info()<<"will cancel control configuration"<<std::endl;
		pop_state();
		return;
	}

	ldi::filter filter(_input());

	using ldi::event;
	using dfw::input_description;

	int wanted=event::source_keyboard | event::source_joystick | event::source_mouse;

	auto ev=filter.find_one(
		wanted,
		event::type_down
	);

	if(ev) {

		switch(ev.source) {

			case event::source_keyboard:
				learn(_input, index, input_description::types::keyboard, ev.code, 0);
			break;
			case event::source_mouse:
				learn(_input, index, input_description::types::mouse, ev.code, 0);
			break;
			case event::source_joystick:
				learn(_input, index, input_description::types::joystick, ev.code, ev.source_index);
			break;
		}
	}
}

void controls::draw(
	ldv::screen& _screen,
	int /*_fps*/
) {

	view.draw(_screen);
}

void controls::learn(
	dfw::input& _input,
	std::size_t _index,
	dfw::input_description::types _type,
    int _code,
	int _device
) {

	entries[_index].input={_type, _code, _device};
	++index;

	if(index >= entries.size()) {

		save_and_finish(_input);
		return;
	}

	view.set_text("target_input", i8n.get(entries.at(index).i8n_key));
}

void controls::save_and_finish(
	dfw::input& _input
) {

	lm::log(logger).info()<<"control configuration ready, will save"<<std::endl;

	for(const auto& node : entries) {

		auto descriptions=_input.locate_descriptions(node.app_id);

		//Remove the last, add the new at the first place. Only the last is
		//removed because there may be defaults, maybe??
		descriptions.pop_back();
		descriptions.insert(std::begin(descriptions), node.input);

		//clear all definitions... use the copy we made to refill it.
		_input.clear(node.app_id);
		for(const auto desc : descriptions) {

			_input.configure({desc, node.app_id});
		}

		//overwrite the configuration now...
		config.set_control_data(
			node.config_map_key,
			node.input.type,
			node.input.code,
			node.input.device
		);
	}

	//save configuration.
	config.save();
	pop_state();
}
