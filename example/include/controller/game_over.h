#pragma once

#include "controller_states.h"
#include "app/service_provider.h"
#include "app/savegame_manager.h"
#include "app/game_session.h"
#include <d2d/components/timeout.h>
#include <d2d/audio/music_player.h> // we need to create silence.
#include <dfw/controller_interface.h>
#include <lm/logger.h>
#include <ldtools/view_composer.h>

namespace controller
{

/**
 * Game over screen, removes the savegame and shows some info. Replaces the
 * game mode, so it stacked on top of the menu on a regular session.
 **/

class game_over:
	public dfw::controller_interface
{
	public:

	                            game_over(app::service_provider&);
	virtual void                loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void                draw(ldv::screen& screen, int);
	virtual void                awake(dfw::input&, int);
	virtual void                slumber(dfw::input&, int) {}
	virtual bool                can_leave_state() const {return true;}

	private:

	//references...
	lm::logger&                 logger;
	ldtools::view_composer      layout;
	d2d::audio::music_player&   music_player;
	app::savegame_manager&      savegame_manager;
	const app::game_session&    game_session;

	//properties
	d2d::components::timeout    timer;
};

}
