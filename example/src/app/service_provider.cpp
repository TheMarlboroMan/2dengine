#include "app/service_provider.h"
#include "app/definitions.h"
#include "app/tile_impl.h"

#include <d2d/collision/shaper_default.h>
#include <d2d/audio/music_player.h>
#include <d2d/video/spritesheet_manager.h>
#include <d2d/video/animation_manager.h>
#include <d2d/video/scenery_tile_draw_animated.h>
#include <d2d/video/sprite_draw.h>
#include <d2d/video/sprite_draw_animated.h>

#include <dfw/id_to_path_mapper.h>

#include <ldtools/ttf_manager.h>

#include <ldv/resource_manager.h>

#include <appenv/env.h>

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

}

service_provider::~service_provider() {

}

const appenv::env& service_provider::get_env() const {

	return env;
}

const dfwimpl::config& service_provider::get_config() const {

	return config;
}

lm::logger& service_provider::get_logger() {

	return logger;
}

d2d::video::spritesheet_manager& service_provider::get_spritesheet_manager() {

	if(nullptr==spritesheet_manager.get()) {

		spritesheet_manager.reset(new d2d::video::spritesheet_manager());
	}

	return *spritesheet_manager;
}

d2d::video::animation_manager& service_provider::get_animation_manager() {

	if(nullptr==animation_manager.get()) {

		animation_manager.reset(new d2d::video::animation_manager());
	}

	return *animation_manager;
}

ldv::resource_manager& service_provider::get_video_resource_manager() {

	return video_resource_manager;
}

lda::resource_manager& service_provider::get_audio_resource_manager() {

	return audio_resource_manager;
}

app::tpersistence& service_provider::get_persistence() {

	if(nullptr==persistence.get()) {

		persistence.reset(new tpersistence());
	}

	return *persistence;
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

		auto music_load_fn=[&](int _id) {

			auto path=get_music_id_mapper().get(_id);
			return env.build_app_path(path);
		};

		music_player->set_load_music_function(music_load_fn);
		music_player->set_unload_finished(true);
	}

	return *music_player;
}

dfw::id_to_path_mapper& service_provider::get_music_id_mapper() {

	if(nullptr==music_id_mapper) {

		music_id_mapper.reset(new dfw::id_to_path_mapper(
			env.build_app_path("resources/lists/music.txt")
		));
	}

	return *music_id_mapper;
}

d2d::video::sprite_draw& service_provider::get_game_sprite_draw() {

	if(nullptr==game_sprite_draw) {

		game_sprite_draw.reset(new d2d::video::sprite_draw(
			get_spritesheet_manager().at(app::ss_tiles),
			get_video_resource_manager().get_texture(app::tex_tiles),
			nullptr,
			false
		));
	}

	return *game_sprite_draw;
}

d2d::video::sprite_draw_animated& service_provider::get_game_sprite_draw_animated() {

	if(nullptr==game_sprite_draw_animated) {

		game_sprite_draw_animated.reset(new d2d::video::sprite_draw_animated(
			get_animation_manager().at(app::animgr_tiles)
		));
	}

	return *game_sprite_draw_animated;
}

d2d::video::scenery_tile_draw_animated& service_provider::get_game_scenery_tile_draw_animated() {

	if(nullptr==game_scenery_tile_draw_animated) {

		game_scenery_tile_draw_animated.reset(new d2d::video::scenery_tile_draw_animated(
			get_spritesheet_manager().at(app::ss_tiles),
			get_video_resource_manager().get_texture(app::tex_tiles),
			app::tile_w, 
			app::tile_h,
			get_animation_manager().at(app::animgr_tiles),
			nullptr,
			false
		));

		//TODO: This kind of shit could come from any file.
		game_scenery_tile_draw_animated->set_is_animation_fn(
			[](int _index) -> bool {
				return _index==app::spr_water_surface || _index==app::spr_waterfall || _index==app::spr_lava_surface;
			}
		);

		//TODO: Same, this should come from a file.
		game_scenery_tile_draw_animated->set_index_to_animation_fn(
			[](int _index) -> int {

				switch(_index) {
					case app::spr_water_surface: return app::anim_water_surface;
					case app::spr_waterfall: return app::anim_waterfall;
					case app::spr_lava_surface: return app::anim_lava_surface;
				}

				return 0;
			}
		);
	}

	return *game_scenery_tile_draw_animated;
}
