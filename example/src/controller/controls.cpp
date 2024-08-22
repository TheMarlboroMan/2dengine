#include "controller/controls.h"
#include "app/input.h"

using namespace controller;

controls::controls(
	app::service_provider& _sp
):
	sp{_sp},
	config{sp.get_config()}
{

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
	dfw::input& _input
) {

	//reset the question array, inputs, flags...
	index=0;
	cancel_signal=false;
	dfw::input_description def{dfw::input_description::types::none, 0, 0};
	for(auto& node : entries) {

		node.input=def;
	}

	//setup a custom callback for the input, so we can learn stuff from it.
	auto fn=[this, &_input](SDL_Event& _event, ldi::sdl_input::tf_default _default) -> bool {

		using dfw::input_description;

		if(SDL_KEYDOWN==_event.type) {

			int scancode=_event.key.keysym.scancode;

			if(SDL_SCANCODE_ESCAPE==scancode) {

				cancel_signal=true;
				return false;
			}

			learn(_input, index, input_description::types::keyboard, scancode, 0);
			return true;
		}

		if(SDL_MOUSEBUTTONDOWN==_event.type) {

			learn(_input, index, input_description::types::mouse, _event.button.button, 0);
			return true;
		}

		if(SDL_JOYBUTTONDOWN==_event.type) {

			auto joyid=_input().get_joystick_index_from_id(_event.jbutton.which);
			learn(_input, index, input_description::types::joystick, _event.jbutton.button, joyid);
			return true;
		}

		_default(_event);
		return true;
	};
	
	_input().set_event_processing_function(fn);
}

void controls::slumber(
	dfw::input& _input
) {

	//Finish "learn" mode.
	_input().reset_event_processing_function();
}

void controls::loop(
	dfw::input& _input,
	const dfw::loop_iteration_data& /*_lid*/
) {

	if(_input().is_exit_signal()) {
		set_leave(true);
		return;
	}

	if(cancel_signal) {

		pop_state();
		return;
	}
}

void controls::draw(
	ldv::screen& /*_screen*/,
	int /*_fps*/
) {

	//TODO: There is no view, no nothing...
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
	}
}

void controls::save_and_finish(
	dfw::input& _input
) {

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

		//overwrite the configuration now... what if there were more than one
		//entries???
		config.set_vector(
			node.config_map_key,
			std::vector<int>{
				dfw::input_description_int_from_type(node.input.type),
				node.input.device, 
				node.input.code
			}
		);
	}

	//save configuration.
	config.save();
	pop_state();
}
