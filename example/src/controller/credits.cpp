#include "controller/credits.h"
#include "app/input.h"
#include "app/definitions.h"

#include <lm/log.h>
#include <tools/string_utils.h>
#include <tools/i8n.h>
#include <ldtools/ttf_manager.h>

using namespace controller;

credits::credits(
	app::service_provider& _sp
)
:
	log{_sp.get_logger()},
	font{_sp.get_ttf_manager().get("credits_font", 8)},
	line_size{16},
	first_y{app::logic_screen_h+2}
{

	lm::log(log).info()<<"will read credits text...\n";

	auto i8n=_sp.get_localization();
	auto text_lines=tools::explode(i8n.get("credits"), '\n');

	int y=first_y;

	for(const auto& l : text_lines) {

		int line_y=y; //always add, to account for empty lines!
		y+=line_size;

		if(0==l.size()) {

			continue;
		}

		lines.push_back({l, line_y});
	}

	lm::log(log).info()<<"created "<<lines.size()<<" line items\n";
}

void credits::awake(
	dfw::input&
) {

	//When rolling we will not use a camera, just move stuff up from their
	//out-of screen position. So we reset everything by knowing how much the
	//first line moved.
	int diff=first_y-lines[0].y;

	for(auto& l : lines) {

		l.y+=diff;
	}

	acc_time=0.;
}

void credits::slumber(
	dfw::input&
) {

	//Remove every single representation that still may remain.
	for(auto& l : lines) {

		l.rep.reset(nullptr);
	}
}

void credits::loop(
	dfw::input& _input,
	const dfw::loop_iteration_data& _lid
) {

	if(_input().is_exit_signal()) {

		set_leave(true);
		return;
	}

	if(_input.is_input_down(app::input::escape)) {

		pop_state();
		return;
	}

	//Are we done?
	const auto& last=lines.back();
	if(last.y <= -128) {

		pop_state();
		return;
	}

	acc_time+=_lid.delta * 20.;

	if(acc_time < 1.) {

		return;
	}

	acc_time-=1.;
	for(auto& l : lines) {

		--l.y;

		if(l.y >= -20 && l.y <= app::logic_screen_h) {

			if(!l.rep) {

				enter_focus(l);
			}
		}
		else {

			if(l.rep) {

				leave_focus(l);
			}
		}

		if(l.rep) {

			auto pos=l.rep->get_position();
			pos.y=l.y;
			l.rep->go_to(pos);
		}
	}
}

void credits::draw(
	ldv::screen& _screen,
	int
) {

	_screen.clear(ldv::rgba8(0, 0, 0, 255));

	for(const auto& l : lines) {

		if(!l.rep) {

			continue;
		}

		l.rep->draw(_screen);
	}
}

credits::line::line(
	const std::string& _text,
	int _y
):
	text{_text},
	y{_y},
	rep{nullptr}
{}

void credits::enter_focus(
	credits::line& _line
) {

	auto text=_line.text;

	//The first char is a color marker...
	char color_marker=text[0];
	auto color=get_color(color_marker);
	text.erase(0, 1); //Remove the first char.

	_line.rep.reset(
		new ldv::ttf_representation(font, color, text)
	);

	//Set y
	_line.rep->go_to({0, _line.y});

	//And align to the right!
	_line.rep->align(
		{0, 0, app::logic_screen_w, app::logic_screen_h},
		{
			ldv::representation_alignment::h::inner_right,
			ldv::representation_alignment::v::none,
			4,
			0
		}
	);
}

ldv::rgba_color credits::get_color(
	char _marker
) {

	//white
	if('*'==_marker) {

		return ldv::rgba8(255, 255, 255, 255);
	}

	if('+'==_marker) {

		return ldv::rgba8(235, 137, 49, 255);
	}

	return ldv::rgba8(49, 162, 242, 255);
}

void credits::leave_focus(
	credits::line& _line
) {

	_line.rep.reset(nullptr);
}
