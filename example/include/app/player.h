#pragma once

#include "entity.h"
#include "definitions.h"
#include <d2d/components/timeouts.h>
#include <d2d/motion/definitions.h>
#include <ldtools/time_definitions.h>

namespace app {

struct ladder;

class player {

	public:

	enum timeout_indexes {
		timeout_ladder=0,
		timeout_last_jump_chance=1,
		timeout_defeat=2,
		timeout_jump_buffer=3
	};

	                                            player();
	entity                                      ent;
	faces                                       facing{faces::right};
	bool                                        jump_shortened{false};
	const app::ladder*                          current_ladder{nullptr};

	void                                        reset();
	void                                        tic(ldtools::tdelta);
	void                                        crouch();
	void                                        stand_up();
	void                                        jump(double);
	void                                        launch(const d2d::motion::motion_vector);
	void                                        defeat(double);
	void                                        start_falling();
	void                                        walk_out_of_ladder(int);
	void                                        jump_out_of_ladder(double, double);
	void                                        drop_out_of_ladder();
	//Only in air state, starts a timer to add another jump input.
	void                                        buffer_jump();

	bool                                        is_crouched() const;
	bool                                        is_defeated() const;
	bool                                        is_defeat_timeout_done() const;
	bool                                        has_jump_last_chance() const;
	//When this timeout is done, ladders can be grabbed again. It resets each time a ladder is grabbed.
	bool                                        is_ladder_timeout_done() const;
	bool                                        has_jump_buffered() const;

//	d2d::collision::box                         get_standing_box() const;

	enum class states {
		ground,
		air,
		ladder,
		crouch,
		defeat
	} state{states::ground};

	private:

	d2d::components::timeouts                   timeouts;

	static const int player_w=12;
	static const int player_h=24;
	static const int player_h_crouch=12;
};
}
