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
};
}
