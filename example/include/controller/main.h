#pragma once

#include <dfw/controller_interface.h>

namespace controller {

class main:
	public dfw::controller_interface {

	public:

	                            main();

	virtual void                loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void                draw(ldv::screen&, int);
	virtual void                awake(dfw::input& /*input*/);
	virtual void                slumber(dfw::input& /*input*/);
	virtual bool                can_leave_state() const {return true;}

	private:
};

}
