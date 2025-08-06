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
		//SKULLS...
		setup_stage_1, //summon skulls and...
		stage_1, //moves left to right shooting directly.
		setup_stage_2, //ready volley counts, change firing time.
		stage_2, //stays still, fires a few more complex volleys. no skulls.
		setup_stage_3, //moves to the center of the screen
		stage_3,  //...
		setup_stage_4, //fires a spiral of stuff.
		stage_4, 
		//SKULLS...
		setup_stage_5, //ready some more skulls!
		stage_5, 
		setup_stage_6, //like stage 2... stay still and fire...
		stage_6, //...but with a bit more dangerous patterns!
		setup_stage_7, //move to the right of the screen...
		stage_7,
		setup_stage_8, //fire a bunch of horizontal volleys!
		stage_8
		//SKULLS...

		//TODO: Third skull phase is last on easy
		//TODO: Fourth skull phase is last on medium
		//TODO: Fifth skull phase is last on hard
	}               stage{stages::appear},
					after_pause_stage{stages::appear};

	void            stage_pause(ldtools::tdelta);
	void            stage_appear(ldtools::tdelta);
	void            setup_stage_one();
	void            stage_one(ldtools::tdelta);
	void            setup_stage_two();
	void            stage_two(ldtools::tdelta);
	void            setup_stage_three();
	void            stage_three(ldtools::tdelta);
	void            setup_stage_four();
	void            stage_four();
	void            setup_stage_five();
	void            stage_five(ldtools::tdelta);
	void            stage_five_hit_skull();
	void            setup_stage_six();
	void            stage_six(ldtools::tdelta);
	void            setup_stage_seven();
	void            stage_seven(ldtools::tdelta);
	void            setup_stage_eight();
	void            stage_eight(ldtools::tdelta);

	void            do_side_movement(ldtools::tdelta, double);
	void            ready_targeted_movement(d2d::collision::point, double);
	bool            do_targeted_movement(ldtools::tdelta);

	//Resumes pause, sets up lengths and next stage.
	void            ready_pause(stages, ldtools::tdelta);

	int             appear_x_start{0},
	                appear_y_target{0},
	                volley_count{0},
	                volley_total{0},
	                volley_angle{0},
	                skull_count{0};
	double          distance_total{0},
	                distance_moved{0};

	d2d::components::timeouts timeouts;

	enum {
		timeout_pause=0,
		timeout_fire,
		timeout_summon_skull
	};
	static const int w{16};
	static const int h{24};
#ifdef IS_DEBUG_BUILD
	static const int appear_y_speed{-34};
#else
	static const int appear_y_speed{-14};
#endif
	static constexpr double phase_one_horizontal_speed{34.};
	static const int phase_one_x_left_limit{42};
	static const int phase_one_x_right_limit{250};
	static const int phase_one_left_offset{-12};
	static const int phase_one_right_offset{26};
	static constexpr double phase_three_speed{20.};
	static constexpr double phase_one_fire_delay{1.8};
	static constexpr double phase_two_fire_delay{1.};
	static constexpr double phase_three_fire_delay{0.4};
	static constexpr double phase_five_horizontal_speed{52.};
	static constexpr double phase_five_volley_delay{1.5};
	static constexpr double phase_five_fire_delay{.15};
	static constexpr double phase_five_summon_skull_delay{4.};
	static constexpr double phase_six_fire_delay{1.};
	static constexpr double phase_seven_speed{25.};
};

std::ostream& operator<<(std::ostream&, const boss&);
}
