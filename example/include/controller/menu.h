#pragma once

#include "app/definitions.h"
#include "app/service_provider.h"
#include "app/env.h"
#include <dfw/controller_interface.h>
#include <lm/logger.h>
#include <ldtools/view_composer.h>

namespace controller {

class menu:
	public dfw::controller_interface {

	public:

	                            menu(app::service_provider&);

	virtual void                loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void                draw(ldv::screen&, int);
	virtual void                awake(dfw::input& /*input*/);
	virtual void                slumber(dfw::input& /*input*/);
	virtual bool                can_leave_state() const {return true;}

	bool                        is_continue_game() const {return continue_game;}
	int                         get_selected_skill() const;
	void                        set_can_continue() {game_can_continue=true;}

	private:

	void                        next();
	void                        prev();
	void                        select();
	void                        back();

	void                        refresh();
	void                        enter_main();
	void                        enter_new_game();
	void                        play_sound(int);

	app::service_provider&      sp;
	const appenv::env&          env;
	lm::logger&                 logger;

	ldtools::view_composer      view;
	enum class levels{main, skill}
	                            curlevel{levels::main};

	enum main_options{
		main_option_choose_skill,
		main_option_continue,
		main_option_exit
	};
	int                         main_option=main_option_choose_skill;

	enum skill_options {
		skill_option_easy,
		skill_option_normal,
		skill_option_hard
	};
	int                         skill_option=skill_option_easy;

	bool                        game_can_continue=false;
	bool                        continue_game=false;
};

}
