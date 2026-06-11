#include "controller/splash.h"
#include "controller/controller_states.h"
#include "app/input.h"
#include "app/env.h"
#include "app/definitions.h"

#include <tools/file_utils.h>
#include <tools/json.h>
#include <tools/i8n.h>
#include <ldtools/ttf_manager.h>
#include <ldv/ttf_representation.h>

using namespace controller;

splash::splash(
	app::service_provider& _sp
):
	timer{1., 0.} //1 second for the first state.
{

	layout.map_font(
		"splash_font", 
		_sp.get_ttf_manager().get("splash_font", 8)
	);

	layout.map_texture(
		"splash", 
		_sp.get_video_resource_manager().get_texture(app::tex_logo)
	);

	const std::string layout_path=_sp.get_env().build_app_path("resources/layout/views.json");
	auto document=tools::parse_json_string(tools::dump_file(layout_path));
	layout.parse(document["splash_screen"]);

	const auto& i8n=_sp.get_localization();
	layout.set_text("a_game_by", i8n.get("splash-a_game_by"));

	auto text=layout.get_by_id("a_game_by");
	auto logo=layout.get_by_id("logo");
	auto center_box=layout.get_by_id("cover_box");
	logo->align(
		*center_box, 
		{
			ldv::representation_alignment::h::center,
			ldv::representation_alignment::v::center
		}
	);

	text->align(
		*logo, 
		{
			ldv::representation_alignment::h::center,
			ldv::representation_alignment::v::outer_top,
			0,
			16
		}
	);
}

void splash::loop(
	dfw::input& _input,
	const dfw::loop_iteration_data& _lid
) {

	if(_input().is_exit_signal()) {

		set_leave(true);
		return;
	}

	if(_input().is_event_input()) {

		force_leave();
	}

	//TODO: If any input, start out!
	timer.tic(_lid.delta);
	if(timer.is_finished()) {

		next();
	}

	auto cover=layout.get_by_id("cover_box");

	switch(state) {

		case states::appear:

			cover->set_alpha(255-(timer.get()*255.) / 2.);

		break;
		case states::dissapear:

			cover->set_alpha((timer.get()*255.) / 2.);
		break;
		case states::begin:
		case states::wait:
		case states::leave:

		break;
	}
}

void splash::draw(
	ldv::screen& _screen,
	int
) {

	layout.draw(_screen);
}

void splash::next() {

	switch(state) {

		case states::begin:

			timer.target(2.0).restart();
			state=states::appear;
			return;

		case states::appear:

			timer.restart();
			state=states::wait;
			return;

		case states::wait:

			timer.restart();
			state=states::dissapear;
			return;

		case states::dissapear:

			timer.target(1.).restart();
			state=states::leave;
			return;

		case states::leave:
			set_state(state_menu);
			return;
	}
}

void splash::force_leave() {

	switch(state) {

		case states::appear:
		case states::wait:

			//Do not touch the timer, everything goes as it goes.
			state=states::dissapear;
			return;

		case states::begin:
		case states::dissapear:
		case states::leave:
			return;
	}
}
