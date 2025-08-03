#pragma once

#include "entity.h"
#include "boss_map_interface.h"
#include <d2d/components/timeouts.h>
#include <ldtools/time_definitions.h>
#include <iostream>

namespace app {

/**
 * game boss. Most stuff about it is hardcoded.
 */
class boss {

	public:

	                boss(d2d::collision::point, int);
	void            tic(ldtools::tdelta);
	void            set_boss_map_interface(boss_map_interface&);
	//Notifies the boss of a skull destroyed and tells about the remaining ones.
	void            notify_skull_destroyed(int);
	void            reset();

	entity          ent;

	private:

	boss_map_interface * bmi{nullptr};

	enum class skills {
		easy, normal, hard
	}               skill;

	enum class stages {
		pause, //actually any pause between phases...
		appear, //comes down from above, after a pause enters stage 1.
		setup_stage_1, //summon skulls and...
		stage_1, //moves left to right shooting directly.
		setup_stage_2, //ready volley counts, change firing time.
		stage_2, //stays still, fires a few more complex volleys. no skulls.
		stage_3, //moves to the center of the screen
		setup_stage_4, //summon skulls and..
		stage_4, //...moves left to right shooting directly
		stage_5, //stays still, I don't know what it does now XD!
		//TODO: Third skull phase is last on easy
		//TODO: Fourth skull phase is last on medium
		//TODO: Fifth skull phase is last on hard
	}               stage{stages::appear},
					after_pause_stage{stages::appear};

	void            stage_appear(ldtools::tdelta);
	void            setup_stage_one();
	void            stage_one(ldtools::tdelta);
	void            setup_stage_two();
	void            stage_two(ldtools::tdelta);
	void            stage_pause(ldtools::tdelta);

	//Resumes pause, sets up lengths and next stage.
	void            ready_pause(stages, ldtools::tdelta);

	int             appear_x_start{0};
	int             appear_y_target{0};
	int             volley_count{0};
	int             volley_total{0};
	d2d::components::timeouts timeouts;

	enum {
		timeout_pause=0,
		timeout_fire
	};
	static const int w{16};
	static const int h{24};
#ifdef IS_DEBUG_BUILD
	static const int appear_y_speed{-34};
#else
	static const int appear_y_speed{-14};
#endif
	static const int horizontal_speed{34};
	static const int phase_one_x_left_limit{16};
	static const int phase_one_x_right_limit{262};
	static constexpr double phase_one_fire_delay{1.8};
	static constexpr double phase_two_fire_delay{1.};
};

std::ostream& operator<<(std::ostream&, const boss&);
}
