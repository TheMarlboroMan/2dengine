#pragma once

#include "entity.h"
#include <d2d/components/timeout.h>
#include <iostream>
#include <ldtools/time_definitions.h>

namespace app {

/**
 * An active timer tics down. When the timer is finished it can activate its
 * target tag and becomes inactive. When its own tag is activated it 
 * "receives a signal" which will:
 * - pause+disable if active and runnning
 * - reset the timer+ disable if active and finished
 * - restart+enable if inactive and finished
 * - resume+enable if inactive and paused.
 */
class timer {

	public:

	                    timer(bool, bool, int, int, int);

	//! Will return true when the tag must be activated!
	bool                tic(ldtools::tdelta);
	bool                is_active() const {return active;}
	int                 get_tag() const {return tag;}
	int                 get_target_tag() const {return target_tag;}
	void                receive_signal();
	void                reset();

	private:

	int                         tag{0},
	                            target_tag{0};
	bool                        active{true},
	                            starting_active{true},
	                            keep_active_when_reset{true};
	d2d::components::timeout    timeout;

	friend std::ostream& operator<<(std::ostream&, const timer&);

};

std::ostream& operator<<(std::ostream&, const timer&);
}

