#pragma once

#include <ldtools/time_definitions.h>
using ldtools::tdelta;

namespace ldv {

	class screen;
}

namespace app {

class background_interface {

	public:

	//!Must draw the background.
	virtual void draw(ldv::screen&)=0;
	//!Must tic the background.
	virtual void tic(tdelta)=0;
	//!Must return the id of a sound to play, zero if none.
	//!Will be called AFTER tic.
	virtual int get_sound() const=0;
};
}
