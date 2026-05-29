#include <app/suspension.h>
#include <ldv/point_representation.h>
#include <algorithm>
#include <cmath>

//TODO
#include <iostream>

using namespace app;

suspension::suspension(
	const player_input& _pli,
	random& _rng,
	int _w,
	int _h,
	int _star_count

):pli{_pli}, rng{_rng}, w{_w}, h{_h}, movement{0.}, time{0.}
{

	points.reserve(_star_count);
	for(int i=0; i<_star_count; i++) {

		points.push_back({0,0,0,0,0,0,0.f});
	}

	shuffle();
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
}

void suspension::draw_foreground(
	ldv::screen&
) {

	//TODO:
}

void suspension::tic(
	tdelta _delta
) {

	auto constexpr two_pi=2 * M_PI;
	time+=_delta;
	if(time >= two_pi) { //Wrap time to one.

		time-=two_pi;
	}

	calculate_motion(_delta);

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

	struct coordinates{
		int x, y;
		bool operator==(const coordinates& _other) const {
			return _other.x==x
			&& _other.y==y;
		}
	};

	//Reposition stars, making sure that no position is repeated. There won't
	//be enough stars as to provoke a deadlock here.
	std::vector<coordinates> existing;
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
