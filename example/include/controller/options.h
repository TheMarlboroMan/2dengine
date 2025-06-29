#pragma once

#include <d2d/components/timeouts.h>
#include <app/service_provider.h>
#include <dfw/controller_interface.h>
#include "dfwimpl/config.h"
#include <ldtools/view_composer.h>
#include <tools/i8n.h>
#include <lm/logger.h>
#include <lda/audio_controller.h>
#include <ldv/screen.h>
#include <string>
#include <vector>

namespace controller {

class options:
	public dfw::controller_interface {

	public:

	                            options(app::service_provider&);

	virtual void                loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void                draw(ldv::screen&, int);
	virtual void                awake(dfw::input& /*input*/);
	virtual void                slumber(dfw::input& /*input*/);
	virtual bool                can_leave_state() const {return true;}

	private:

	enum {
		option_display=0,
		option_sound,
		option_music,
		option_back
	};

	void                        refresh_index();
	void                        update_values();
	void                        select_option(int);
	void                        select_display();
	void                        select_sound(int);
	void                        select_music(int);
	int                         volume_to_gui(int) const;
	int                         gui_to_volume(int) const;

	app::service_provider&      sp;
	lm::logger&                 logger;
	dfwimpl::config&            config;
	const tools::i8n&           i8n;
	lda::audio_controller&      audio_c;
	ldv::screen&                screen;
	d2d::components::timeout    enter_timeout;
	ldtools::view_composer      view;
	std::size_t                 index{0};
	int                         consecutive_press_tics{0};
};

}
