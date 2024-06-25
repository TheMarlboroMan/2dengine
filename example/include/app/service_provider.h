#pragma once

#include "app/tile_impl.h"
#include "types.h"
#include <memory>
//Sadly a forward declaration it not enough for the unique_ptr...
#include <d2d/collision/shaper.h>
#include <ldtools/ttf_manager.h>
#include <d2d/video/spritesheet_manager.h>
#include <d2d/video/animation_manager.h>
#include <d2d/audio/music_player.h>
#include <ldv/resource_manager.h>
#include <dfw/kernel.h>
#include <appenv/env.h>

namespace lm {
class logger;
}

namespace dfwimpl{
class config;
}

namespace ldtools{
class ttf_manager;
}
/**
 
namespace d2d{namespace collision{
class shaper;
}};
*/

namespace app {

class service_provider {

	public:

	service_provider(const appenv::env&, const dfwimpl::config&, lm::logger&, dfw::kernel&);
	~service_provider();

	const appenv::env&		get_env() const {return env;}
	const dfwimpl::config&	get_config() const {return config;}
	lm::logger&				get_logger() {return logger;}
	d2d::collision::shaper& get_shaper();
	const app::tile_impl&   get_tile_impl();
	ldtools::ttf_manager&   get_ttf_manager();
	d2d::video::spritesheet_manager& get_spritesheet_manager() {return spritesheet_manager;}
	d2d::video::animation_manager& get_animation_manager() {return animation_manager;}
	ldv::resource_manager&  get_video_resource_manager() {return video_resource_manager;}
	app::tpersistence&      get_persistence() {return persistence;}
	d2d::audio::music_player& get_music_player();

	private:

	const appenv::env&		env;
	const dfwimpl::config&	config;
	lm::logger&				logger;
	ldv::resource_manager&	video_resource_manager;
	lda::resource_manager&	audio_resource_manager;
	dfw::audio&             dfwaudio;
	std::unique_ptr<d2d::collision::shaper>	shaper{nullptr};
	std::unique_ptr<app::tile_impl>	tile_impl{nullptr};
	std::unique_ptr<ldtools::ttf_manager>	ttf_manager{nullptr};
	std::unique_ptr<d2d::audio::music_player> music_player{nullptr};
	d2d::video::spritesheet_manager spritesheet_manager;
	d2d::video::animation_manager   animation_manager;
	tpersistence                    persistence;
};
}
