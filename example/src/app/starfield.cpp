#include "app/starfield.h"
#include "app/random.h"

#include <ldv/screen.h>
#include <ldv/point_representation.h>

using namespace app;

starfield::starfield(
	int _w,
	int _h,
	int _count,
	random& _rng
): w{_w}, h{_h}, time{0.0}, pulse_time{0.0}, rng{_rng} {

	//This is the shape of a key.
	struct p{int x, y;};
	std::vector<p> pts{
		{150, 150},
		{142, 138},
		{137, 125},
		{145, 115},
		{161, 146},
		{161, 127},
		{180, 120},
		{201, 112},
		{220, 103},
		{222, 112},
		{229, 118},
		{205, 117}
	};

	static_stars.reserve(pts.size());
	for(const auto pt: pts) {

		static_stars.push_back({pt.x, pt.y, 0, 0, 0});
	}

	//count better be positive...
	moving_stars.reserve(_count);
	for(int i=0; i<_count; i++) {

		int x=rng.get(1, w-1);
		int y=rng.get(1, h-1);
		short int max_velocity=rng.get(1, 16);
		short int variant=rng.get(0, 3);

		moving_stars.push_back({x, y, max_velocity, max_velocity, variant});
	}
}

void starfield::tic(
	tdelta _delta
) {

	time+=_delta;
	pulse_time+=_delta;
	if(time < 0.025) {

		return;
	}

	time=0.;

	for(auto& dot : moving_stars) {

		--dot.velocity;
		if(0==dot.velocity) {

			--dot.y;
			dot.velocity=dot.max_velocity;

			if(dot.y <= 0) {

				dot.x=rng.get(1, w-1);
				dot.y=h;
			}
		}

	}
}

void starfield::draw(
	ldv::screen& _screen
) {

	//We should be able to create a single representation and add all points
	//but that's quite miserable since multi-point representation have 
	//lots of issues.

	//No tricks, these are all colours of our palette...
	ldv::rgba_color variants[]={
		ldv::rgba8(255,255,255,255),
		ldv::rgba8(157, 157, 157, 255),
		ldv::rgba8(27, 38, 50,255),
		ldv::rgba8(178, 220, 239, 255)
	};

	for(const auto& dot : moving_stars) {

		ldv::point_representation pt{
			{dot.x, dot.y},
			variants[dot.variant]
		};
		pt.draw(_screen);
	}


	for(const auto& dot : static_stars) {

		//This is a trick, we don't have as many reds in our palette.
		int pulse=sin(pulse_time)*60;
		int red=180+pulse;

		ldv::point_representation pt{
			{dot.x, dot.y},
			ldv::rgba8(red,0,0,255)
		};
		pt.draw(_screen);
	}
}
