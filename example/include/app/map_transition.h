#pragma once

#include <ldtools/time_definitions.h>
#include "exit.h"

namespace ldv {

class screen;
}

namespace app {

using ldtools::tdelta;

/**
 * Interface for transition objects. These can be created when exiting from 
 * a map to another. We assume a transition is some sort of "animation" that
 * makes the level change a bit prettier, has a start, an end and is time bound
 * so it must be tic each frame.
 */
class map_transition {

	public:

	//exit taken on the original map.
	virtual const app::exit get_original_exit() const=0;

	//tics the transition.
	virtual void tic(tdelta)=0;

	//must return true if the transition is finished.
	virtual bool is_finished() const=0;

	//must draw the transition. This will get called after every game thing
	//is drawn!
	virtual void draw(ldv::screen&) const=0;
};
}
