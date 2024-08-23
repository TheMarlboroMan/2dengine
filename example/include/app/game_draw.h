#pragma once

#include <d2d/video/scenery_tile_draw_animated.h>
#include <d2d/video/sprite_draw.h>
#include <d2d/video/sprite_draw_animated.h>
#include <ldtools/ttf_manager.h>
#include <ldv/screen.h>
#include <ldv/camera.h>
#include <ldv/ttf_representation.h>
#include <ldv/box_representation.h>
#include <string>

namespace app {

class map;
class player;
class ladder;
class collectible;
class linear_monster;
class leaping_monster;
class timed_trap;
class secret_cover;
class button;
class gate;
class projectile;
class breaking_platform;
class platform_block;

/**
 * business class to draw the main game state.
 */
class game_draw {

	public:

	                            game_draw(
		ldv::camera&,
		d2d::video::scenery_tile_draw_animated&, 
		d2d::video::sprite_draw&,
		d2d::video::sprite_draw_animated&,
		ldtools::ttf_manager&
	);
	                            ~game_draw();

	void                        draw(ldv::screen&, const app::map&, const app::player&);
	void                        draw_area_name_banner(ldv::screen&);
	void                        draw_lives_banner(ldv::screen&);

	void                        setup_area_name_banner(const std::string&);

	private:

	ldv::camera&                camera;
	d2d::video::scenery_tile_draw_animated&  scenery_tile_draw;
	d2d::video::sprite_draw&        sprite_draw;
	d2d::video::sprite_draw_animated& sprite_draw_animated;
	ldv::ttf_representation     area_name_banner_text;
	ldv::box_representation     area_name_banner_background;

	void                        draw_player(ldv::screen&, const app::player&);
	void                        draw_ladder(ldv::screen&, const app::ladder&);
	void                        draw_collectible(ldv::screen&, const app::collectible&);
	void                        draw_linear_monster(ldv::screen&, const app::linear_monster&);
	void                        draw_leaping_monster(ldv::screen&, const app::leaping_monster&);
	void                        draw_timed_trap(ldv::screen&, const app::timed_trap&);
	void                        draw_secret_cover(ldv::screen&, const app::secret_cover&);
	void                        draw_button(ldv::screen&, const app::button&);
	void                        draw_gate(ldv::screen&, const app::gate&);
	void                        draw_projectile(ldv::screen&, const app::projectile&);
	void                        draw_projectile_linear(ldv::screen&, const app::projectile&);
	void                        draw_projectile_directed(ldv::screen&, const app::projectile&);
	void                        draw_breaking_platform(ldv::screen&, const app::breaking_platform&);
	void                        draw_platform(ldv::screen&, const app::platform_block&);
};
}
