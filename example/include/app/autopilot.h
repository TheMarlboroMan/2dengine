#pragma once

#include "player_input.h"
#include <d2d/components/timeout.h>

namespace app {

/**
 * the autopilot can produce input automatically to render cutscenes using
 * the main game loop
 */
class autopilot {

	public:

	enum inputs {
		none=0,
		up=1,
		right=2,
		down=4,
		left=8,
		jump=16,
		pressed=32 //modifier to up and jump
	};

                    autopilot();

	//! Returns true if the autopilot is active.
	bool            is_enabled() const {return enabled;}
	//! Receives input for the autopilot with an integer, each bit representing a flag. Automatically enables it. The delta value is an optional pause.
	void            receive(int, ldtools::tdelta=0.);
	//! Disables the autopilot.
	void            disable();
	//! Fills the player input structure so that it can be played by the game.
	void            produce(player_input&);
	//! Tics the autopilot, which is important if pauses are involved.
	void            tic(ldtools::tdelta);

	private:

	bool            enabled{false};
	int             current_produce{0};
	d2d::components::timeout timer;


};
}
