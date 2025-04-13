#pragma once

#include "types.h"
#include <memory>
#include <dfw/kernel.h>

namespace appenv {
	class env;
};

namespace ldv {
	class resource_manager;
}

namespace lm {
	class logger;
}

namespace dfwimpl{
	class config;
}

namespace dfw {
	class id_to_path_mapper;
}

namespace ldtools{
	class ttf_manager;
}

namespace tools {
	class i8n;
};

namespace d2d{

	namespace video {
		class spritesheet_manager;
		class animation_manager;
		class sprite_draw;
		class animation_sprite_finder;
		class scenery_tile_draw;
	}

	namespace audio {
		class music_player;
		class sound_player;
	}

	namespace collision{
		class shaper;
	}
};

namespace app {

class tile_impl;
class automap;
class inventory;
class game_session;

class service_provider {

	public:

	service_provider(const appenv::env&, dfwimpl::config&, lm::logger&, dfw::kernel&);
	~service_provider();

//TODO: Review, some of these might just be private.
	const appenv::env&		get_env() const;
	dfwimpl::config&	    get_config();
	lm::logger&				get_logger();
	d2d::collision::shaper& get_shaper();
	const app::tile_impl&   get_tile_impl();
	ldtools::ttf_manager&   get_ttf_manager();
	dfw::audio&             get_audio() {return dfwaudio;}

	d2d::video::spritesheet_manager& get_spritesheet_manager();
	d2d::video::animation_manager& get_animation_manager();
	ldv::resource_manager&  get_video_resource_manager();
	lda::resource_manager&  get_audio_resource_manager();
	app::tpersistence&      get_persistence();
	d2d::audio::music_player& get_music_player();
	d2d::audio::sound_player& get_sound_player();
	d2d::video::sprite_draw& get_game_sprite_draw();
	d2d::video::animation_sprite_finder& get_game_animation_sprite_finder();
	d2d::video::scenery_tile_draw& get_game_scenery_tile_draw();
	tools::i8n&             get_localization();
	automap&                get_automap();
	inventory&              get_inventory();
	game_session&           get_game_session();

	private:

	dfw::id_to_path_mapper& get_music_id_mapper();

	const appenv::env&		env;
	dfwimpl::config&	    config;
	lm::logger&				logger;
	ldv::resource_manager&	video_resource_manager;
	lda::resource_manager&	audio_resource_manager;
	dfw::audio&             dfwaudio;

	std::unique_ptr<d2d::collision::shaper>	shaper{nullptr};
	std::unique_ptr<app::tile_impl>	tile_impl{nullptr};
	std::unique_ptr<ldtools::ttf_manager>	ttf_manager{nullptr};
	std::unique_ptr<d2d::audio::music_player> music_player{nullptr};
	std::unique_ptr<d2d::audio::sound_player> sound_player{nullptr};
	std::unique_ptr<dfw::id_to_path_mapper> music_id_mapper{nullptr};
	std::unique_ptr<d2d::video::spritesheet_manager> spritesheet_manager{nullptr};
	std::unique_ptr<d2d::video::animation_manager> animation_manager{nullptr};
	std::unique_ptr<tpersistence> persistence{nullptr};
	std::unique_ptr<d2d::video::sprite_draw> game_sprite_draw{nullptr};
	std::unique_ptr<d2d::video::animation_sprite_finder> game_animation_sprite_finder{nullptr};
	std::unique_ptr<d2d::video::scenery_tile_draw> game_scenery_tile_draw{nullptr};
	std::unique_ptr<tools::i8n> localization{nullptr};
	std::unique_ptr<automap> game_automap{nullptr};
	std::unique_ptr<inventory> game_inventory{nullptr};
	std::unique_ptr<game_session> session{nullptr};
};
}
