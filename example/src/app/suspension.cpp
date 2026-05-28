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

):pli{_pli}, rng{_rng}, w{_w}, h{_h}, movement{0.}
{

	points.reserve(_star_count);
	for(int i=0; i<_star_count; i++) {

		points.push_back({0,0,0,0});
	}

	shuffle();
}


void suspension::draw_background(
	ldv::screen& _screen
) {

	for(const auto& dot : points) {

		ldv::point_representation pt{
			{dot.x+dot.dispersion, dot.y},
			ldv::rgba8(255,255,255,255)
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
