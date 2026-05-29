#include <app/suspension.h>
#include <ldv/point_representation.h>
#include <d2d/video/tools.h>
#include <algorithm>
#include <cmath>

using namespace app;

suspension::suspension(
	const player_input& _pli,
	std::vector<std::string> _texts,
	random& _rng,
	const ldv::camera& _camera,
	const double& _tracked_y,
	const ldv::ttf_font& _font,
	int _star_count
):pli{_pli}, rng{_rng}, camera{_camera},
	tracked_y{_tracked_y},
	movement{0.}, time{0.},
	texts{std::move(_texts)}, 
	text_timeout{0, 0., false},
	current_text_index{0},
	text_font{_font},
	text_x{0.},
	current_text{nullptr}
{

	points.reserve(_star_count);
	for(int i=0; i<_star_count; i++) {

		points.push_back({0,0,0,0,0,0,0.f});
	}

	//The shape of yet another key
	fixed={
		{170, 127},
		{165, 123},
		{177, 125},
		{176, 119},
		{171, 116},
		{174, 109},
		{175, 100},
		{177, 92},
		{177, 84},
		{179, 76},
		{186, 77},
		{186, 79},
		{182, 84},
	};

	shuffle();

	//Set the first timeout...
	text_timeout.target(rng.get(6, 8));
}

void suspension::draw_background(
	ldv::screen& _screen
) {

	ldv::rgba_color variants[]={
		ldv::rgba8(255,255,255,255),
		ldv::rgba8(157, 157, 157, 255),
		ldv::rgba8(27, 38, 50,255),
		ldv::rgba8(178, 220, 239, 255)
	};

	for(const auto& dot : points) {

		int x=dot.x+dot.dispersion;
		int y=dot.y + std::floor(sin(time+dot.time_displacement) * dot.amplitude);

		ldv::point_representation pt{
			{x, y},
			variants[dot.variant]
		};
		pt.draw(_screen);
	}

	double alpha=255.*key_alpha;
	auto color=ldv::rgba8(68, 137, 26, alpha);
	for(const auto& dot : fixed) {

		ldv::point_representation pt{
			{dot.x, dot.y},
			color
		};
		pt.set_blend(ldv::representation::blends::alpha);
		pt.draw(_screen);
	}
}

void suspension::draw_foreground(
	ldv::screen& _screen
) {

	if(!current_text) {

		return;
	}

	current_text->draw(_screen);
}

void suspension::tic(
	tdelta _delta
) {

	tic_text(_delta);

	auto constexpr two_pi=2 * M_PI;
	time+=_delta;
	if(time >= two_pi) { //Wrap time to one.

		time-=two_pi;
	}

	calculate_motion(_delta);
	calculate_alpha(_delta);

	for(auto& pt : points) {

		//now, movement goes from -1 to 1. 0 is no dispersion,  -1 is
		//-max_dispersion and 1 is max_dispersion
		pt.dispersion=movement * pt.max_dispersion;
	}
}

int suspension::get_sound() const {

	return 0;
}

void suspension::shuffle() {

	//Reposition stars, making sure that no position is repeated. There won't
	//be enough stars as to provoke a deadlock here.
	std::vector<coordinates> existing;
	std::copy(std::begin(fixed), std::end(fixed), std::back_inserter(existing));

	const auto& camera_pos_box=camera.get_pos_box();
	int w=camera_pos_box.w,
	    h=camera_pos_box.h;

	for(std::size_t i=0; i<points.size();) {

		coordinates pt={
			rng.get(1, w-1),
			rng.get(1, h-1)
		};

		if(0!=std::count(std::begin(existing), std::end(existing), pt)) {

			continue;
		}

		existing.push_back(pt);

		auto& curpt=points[i];
		curpt.x=pt.x;
		curpt.y=pt.y;
		curpt.max_dispersion=rng.get(10, 40);
		curpt.dispersion=0;
		curpt.amplitude=rng.get(1, 10);
		curpt.variant=rng.get(0, 3);

		float fraction=(float)rng.get(0, 10) / 10.f;
		curpt.time_displacement=1.f-(fraction / 5.f);

		i++;
	}
}

void suspension::calculate_motion(
	tdelta _delta
) {

	if(0==pli.x) {

		//Almost about to stop? Don't stand for endless noddling around zero,
		//bring it to zero (this would go to -0.00003 to 0.0003 endlessly).
		if(std::abs(movement) <= 0.05 && 0==pli.x) {

			movement=0.;
			return;
		}

		if(0.==movement) {

			return;
		}
	}


	//Calculate the sign as the opposite of the input or whatever brings it
	//towards zero if no input is given.
	auto sign=pli.x==1 ? -1.0 : 1.0;
	if(0==pli.x) {
		sign=movement > 0 ? -1. : 1.;
	}

	constexpr double speed=.5;
	movement+=speed * _delta * sign; //Takes one second to reach full speed.
	movement=std::clamp(movement, -1., 1.);
}

void suspension::calculate_alpha(
	tdelta _delta
) {

	constexpr double speed=1. / 5.; //Should take 5 seconds to reach max.

	if(!pli) {

		key_alpha-=_delta * speed;

		if(key_alpha <= 0.05) {

			key_alpha=0.;
			return;
		}
	}
	else {

		key_alpha+=_delta * speed;
	}

	key_alpha=std::clamp(key_alpha, 0., 1.);
}

void suspension::tic_text(
	tdelta _delta
) {

	if(!current_text) {

		text_timeout.tic(_delta);
		if(text_timeout.is_finished()) {

			if(current_text_index >= texts.size()) {

				return;
			}

			current_text.reset(
				new ldv::ttf_representation(
					text_font,
					ldv::rgba8(255, 255, 255, 255),
					texts[current_text_index++]
				)
			);

			//The X is absolute...
			text_x=camera.get_pos_box().w;
		}

		if(!current_text) { //if by now we haven't instantiaded, leave.

			return;
		}
	}

	text_x-=_delta * 50.;

	//16 is track a bit upwards from the player... Also that tracked y is
	//world position so we need to adjust for camera.
	double y=camera.get_pos_box().h-(tracked_y+16-camera.get_y());

	ldv::point pos{(int)text_x, (int)y};
	current_text->go_to(pos);

	//Remove when the text goes out of view.
	const auto& text_pos=current_text->get_text_position();
	auto end=text_pos.origin.x+text_pos.w;

	if(0 >= end) {

		current_text.reset(nullptr);
		text_timeout.target(rng.get(3, 6));
		text_timeout.restart();
	}
}
