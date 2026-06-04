#pragma once

#include "controller_states.h"
#include "app/service_provider.h"
#include <dfw/controller_interface.h>
#include <lm/logger.h>
#include <ldv/ttf_representation.h>
#include <ldv/ttf_font.h>
#include <string>
#include <vector>
#include <memory>
#include <ldv/color.h>

namespace controller
{

class credits:
	public dfw::controller_interface
{
	public:

	                            credits(app::service_provider&);
	virtual void                loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void                draw(ldv::screen& screen, int);
	virtual void                awake(dfw::input&);
	virtual void                slumber(dfw::input&);
	virtual bool                can_leave_state() const {return true;}

	private:

	class line {

		public:

		     line(const std::string&, int);

		std::string text;
		int y;
		std::unique_ptr<ldv::ttf_representation> rep;
	};

	//Methods for the line entering and leaving the screen
	void                        enter_focus(line&);
	void                        leave_focus(line&);
	ldv::rgba_color             get_color(char);


	//references...
	lm::logger&                 log;
	const ldv::ttf_font&        font;

	//properties.
	std::vector<line>           lines;
	const int                   line_size; //How much y we will add between lines.
	const int                   first_y; //This will be the y of the first string.
	double                      acc_time{0.};
};

}
