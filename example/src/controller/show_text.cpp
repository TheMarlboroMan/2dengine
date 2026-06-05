#include "controller/show_text.h"
#include "app/input.h"

#include <lm/log.h>
#include <tools/json.h>
#include <tools/string_utils.h>
#include <tools/file_utils.h>
#include <ldv/ttf_representation.h>
#include <ldv/color.h>

using namespace controller;

show_text::show_text(
	app::service_provider& _sp
):
	log{_sp.get_logger()},
	text_exchange{_sp.get_show_text_exchange()},
	current_index(0)
{

	timers.add(time_text, 0.05, 0., true);
	timers.add(time_blink, 2.0, 0., true);

	layout.map_font(
		"text_font", 
		_sp.get_ttf_manager().get("show_text_font", 8)
	);

	const std::string layout_path=_sp.get_env().build_app_path("resources/layout/views.json");
	auto document=tools::parse_json_string(tools::dump_file(layout_path));
	layout.parse(document["text_display"]);
}

void show_text::awake(
	dfw::input&
) {

	raw_text=text_exchange.get();
	reload_buffer();
}

void show_text::loop(
	dfw::input& _input,
	const dfw::loop_iteration_data& _lid
) {

	timers.tic(_lid.delta);

	if(_input().is_exit_signal()) {

		set_leave(true);
		return;
	}
	else if(_input.is_input_down(app::input::escape)) {

		finish();
		return;
	}

	bool is_action=_input.is_input_down(app::input::jump)
		|| _input.is_input_down(app::input::pause);

	if(timers.is_finished(time_blink)) {

		timers.restart(time_blink);
	}

	switch(state) {

		case tstates::dictate:

			if(is_action) {

				current_index=std::begin(text_buffer)->size();
				redraw_text();
			}
			else {

				if(timers.is_finished(time_text)) {

					++current_index;
					if(current_index >= std::begin(text_buffer)->size()) {

						current_index=std::begin(text_buffer)->size();
						state=tstates::wait;
						timers.restart(time_blink);
					}

					timers.restart(time_text);
					redraw_text();
				}
			}

		break;

		case tstates::wait:

			if(is_action) {

				text_buffer.erase(std::begin(text_buffer));
				if(!text_buffer.size()) {

					finish();
					return;
				}
				else {

					timers.restart();
					current_index=0;
					state=tstates::dictate;
					redraw_text();
				}
			}
		break;
	}
}

void show_text::draw(
	ldv::screen& _screen,
	int /*fps*/
) {

	if(state==tstates::wait) {

		bool visible=fmod(timers.get(time_blink), 2.) >= 0.5;
		layout.get_by_id("wait_text")->set_visible(visible);
	}
	else {

		layout.get_by_id("wait_text")->set_visible(false);
	}

	layout.draw(_screen);
}

void show_text::request_draw(
	dfw::controller_view_manager& _cvm
) {

	_cvm.add(state_main);
	_cvm.add_ptr(this);
}

void show_text::redraw_text() {

	auto rep=static_cast<ldv::ttf_representation *>(
		layout.get_by_id("main_text")
	);

	rep->set_text(std::begin(text_buffer)->substr(0, current_index));

	switch(text_exchange.get_colour()) {
		case app::show_text_exchange::white:
		default:
			rep->set_color(ldv::rgba8(255, 255, 255, 255));
		break;
		case app::show_text_exchange::orange:
			rep->set_color(ldv::rgba8(235, 137, 49, 255));
		break;
		case app::show_text_exchange::red:
			rep->set_color(ldv::rgba8(190, 38, 51, 255));
		break;
		case app::show_text_exchange::blue:
			rep->set_color(ldv::rgba8(49, 162, 242, 255));
		break;
		case app::show_text_exchange::green:
			rep->set_color(ldv::rgba8(68, 137, 26, 255));
		break;
	}
}

void show_text::reload_buffer() {

	text_buffer=tools::explode(raw_text, "[@np]");
	for(auto& str : text_buffer) {
		str=tools::str_trim(str);
	}

	current_index=0;
	state=tstates::dictate;
	timers.restart();
	redraw_text();
}

void show_text::finish() {

	//Return to the game controller if no question is to be asked...
	if(!text_exchange.has_answers()) {

		lm::log(log).info()<<"no answers popping state\n";
		pop_state();
		return;
	}

	lm::log(log).info()<<"there are answers, will enter the question state\n";
	push_state(controller::state_question);
}
