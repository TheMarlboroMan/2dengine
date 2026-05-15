#include "app/starfield.h"
#include "app/random.h"

using namespace app;

starfield::starfield(
	int _w,
	int _h,
	int _count,
	random& _rng
): w{_w}, h{_h}, time{0.0}, rng{_rng} {

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

		static_stars.push_back({pt.x, pt.y, 0, 0});
	}

	//count better be positive...
	moving_stars.reserve(_count);
	for(int i=0; i<_count; i++) {

		moving_stars.push_back({0,0,0,0});
	}
}

void starfield::tic(
	tdelta _delta
) {

	time+=_delta;
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

void starfield::reload() {

	for(auto& dot : moving_stars) {

		dot.x=rng.get(1, w-1);
		dot.y=rng.get(1, h-1);
		dot.velocity=rng.get(1, 16);
		dot.max_velocity=dot.velocity;
	}
}
