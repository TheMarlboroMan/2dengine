#pragma once

#include "app/definitions.h"
#include "app/service_provider.h"
#include "app/env.h"
#include "app/savegame_manager.h"
#include <dfw/controller_interface.h>
#include <lm/logger.h>
#include <ldtools/view_composer.h>
#include <string>

namespace controller {

class menu:
	public dfw::controller_interface {

	public:

	enum class signals {
		new_game,
		continue_game,
		load_game
	};

	                            menu(app::service_provider&);

	virtual void                loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void                draw(ldv::screen&, int);
	virtual void                awake(dfw::input& /*input*/);
	virtual void                slumber(dfw::input& /*input*/);
	virtual bool                can_leave_state() const {return true;}

	signals                     get_action_signal() const {return action_signal;}
	int                         get_selected_skill() const;
	const std::string&          get_slot_filename() const;
	//make the "continue" option available.
	void                        set_can_continue() {game_can_continue=true;}

	private:

	void                        next();
	void                        prev();
	void                        select();
	void                        remove();
	void                        back();
	//Do this when entering these menus...
	void                        enter_main();
	void                        enter_slot_select();
	void                        enter_skill_select();

	//Ready views..
	void                        ready_main();
	void                        ready_slot_select();
	void                        ready_skill_select();
	//Specific actions.
	void                        attempt_to_continue();
	void                        choose_slot();
	void                        choose_skill();
	void                        start_new_game();
	void                        load_game();
	//helpers
	void                        refresh();
	void                        refresh_save_slots();
	void                        set_savegame_description(int);
	void                        play_sound(int);

	app::service_provider&      sp;
	const appenv::env&          env;
	lm::logger&                 logger;

	app::savegame_manager       savegame_manager;
	ldtools::view_composer      view;
	enum class levels{main, slot, skill}
	                            curlevel{levels::main};

	enum main_options{
		main_option_choose_slot,
		main_option_continue,
		main_option_exit
	};
	int                         main_option=main_option_choose_slot;

	enum skill_options {
		skill_option_easy,
		skill_option_normal,
		skill_option_hard
	};
	int                         skill_option=skill_option_easy;

	enum slot_options {
		slot_one,
		slot_two,
		slot_three
	};
	int                         slot_option=slot_one;

	signals                     action_signal{signals::new_game};
	bool                        game_can_continue=false;
};

}
