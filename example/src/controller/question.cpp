#include "controller/question.h"
#include "app/input.h"
#include "app/env.h"

#include <lm/log.h>
#include <tools/file_utils.h>
#include <tools/json.h>
#include <ldtools/ttf_manager.h>
#include <ldv/ttf_representation.h>

using namespace controller;

question::question(
	app::service_provider& _sp
)
:
	log{_sp.get_logger()},
	text_exchange{_sp.get_show_text_exchange()},
	current_index(0)
{

	layout.map_font(
		"text_font", 
		_sp.get_ttf_manager().get("show_text_font", 8)
	);

	const std::string layout_path=_sp.get_env().build_app_path("resources/layout/views.json");
	auto document=tools::parse_json_string(tools::dump_file(layout_path));
	layout.parse(document["question_display"]);

	const auto& i8n=_sp.get_localization();
	layout.set_text("first_question", i8n.get("question-first"));
	layout.set_text("second_question", i8n.get("question-second"));
	layout.set_text("third_question", i8n.get("question-third"));
}

void question::awake(
	dfw::input&
) {

	current_index=0;
	update();
}

void question::loop(
	dfw::input& _input,
	const dfw::loop_iteration_data&
) {

	if(_input().is_exit_signal()) {

		set_leave(true);
		return;
	}

	if(_input.is_input_down(app::input::up)) {

		previous();
	}
	else if(_input.is_input_down(app::input::down)) {

		next();
	}
	else if(_input.is_input_down(app::input::jump)) {

		finish();
	}
}

void question::draw(
	ldv::screen& _screen,
	int
) {

	layout.draw(_screen);
}

void question::request_draw(
	dfw::controller_view_manager& _cvm
) {

	_cvm.add(state_main);
	_cvm.add_ptr(this);
}

void question::finish() {

	//Set the answer for the text controller to pick up next.
	//The text controller won't go back here when the text finishes
	//because all answers will be cleared.
	switch(current_index) {
		case 0: text_exchange.answer_first(); break;
		case 1: text_exchange.answer_second(); break;
		case 2: text_exchange.answer_third(); break;
	}

	text_exchange.clear_answers();
	pop_state();
}

void question::next() {

	if(current_index==2) {

		return;
	}

	++current_index;
	update();
}

void question::previous() {

	if(0==current_index) {

		return;
	}

	--current_index;
	update();
}

void question::update() {

	auto color_first=current_index == 0
		? ldv::rgba8(255, 255, 255, 255)
		: ldv::rgba8(128, 128, 128, 255);

	auto color_second=current_index == 1
		? ldv::rgba8(255, 255, 255, 255)
		: ldv::rgba8(128, 128, 128, 255);

	auto color_third=current_index == 2
		? ldv::rgba8(255, 255, 255, 255)
		: ldv::rgba8(128, 128, 128, 255);

	static_cast<ldv::ttf_representation *>(layout.get_by_id("first_question"))
		->set_color(color_first);

	static_cast<ldv::ttf_representation *>(layout.get_by_id("second_question"))
		->set_color(color_second);

	static_cast<ldv::ttf_representation *>(layout.get_by_id("third_question"))
		->set_color(color_third);
}
