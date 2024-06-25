#include "app/service_provider.h"
#include "app/definitions.h"
#include <d2d/collision/shaper_default.h>
#include <ldtools/ttf_manager.h>

using namespace app;

service_provider::service_provider(
	const appenv::env& _env, 
	const dfwimpl::config& _config,
	lm::logger& _logger,
	dfw::kernel& _kernel
):
	env{_env},
	config{_config},
	logger{_logger},
	video_resource_manager{_kernel.get_video_resource_manager()},
	audio_resource_manager{_kernel.get_audio_resource_manager()},
	dfwaudio{_kernel.get_audio()}
{ 

	persistence.add(app::pergr_collectibles);
	persistence.add(app::pergr_secret_covers);
	persistence.add(app::pergr_buttons);
	persistence.add(app::pergr_touch_triggers);
}

service_provider::~service_provider() {

}

d2d::collision::shaper& service_provider::get_shaper() {

	if(nullptr==shaper.get()) {

		shaper.reset(new d2d::collision::shaper_default(app::tile_w, app::tile_h));
	}

	return *shaper;
}

const app::tile_impl& service_provider::get_tile_impl() {

	if(nullptr==tile_impl.get()) {

		tile_impl.reset(new app::tile_impl());
	}

	return *tile_impl;
}


ldtools::ttf_manager& service_provider::get_ttf_manager() {

	if(nullptr==ttf_manager.get()) {

		ttf_manager.reset(new ldtools::ttf_manager{});
	}

	return *ttf_manager;
}

d2d::audio::music_player& service_provider::get_music_player() {

	if(nullptr==music_player.get()) {

		music_player.reset(new d2d::audio::music_player(get_logger(), dfwaudio, audio_resource_manager));
	}

	return *music_player;
}
