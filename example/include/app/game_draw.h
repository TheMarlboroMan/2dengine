#pragma once

#include "app/env.h"
#include <d2d/video/scenery_tile_draw.h>
#include <d2d/video/sprite_draw.h>
#include <d2d/video/sprite_fill_draw.h>
#include <d2d/video/animation_sprite_finder.h>
#include <ldtools/ttf_manager.h>
#include <ldtools/view_composer.h>
#include <ldv/screen.h>
#include <ldv/camera.h>
#include <ldv/ttf_representation.h>
#include <ldv/box_representation.h>
#include <ldv/bitmap_representation.h>
#include <ldv/resource_manager.h>
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
class moving_block;
class facing_block;

/**
 * business class to draw the main game state.
 */
class game_draw {

	public:

	                            game_draw(
		ldv::camera&,
		d2d::video::scenery_tile_draw&, 
		d2d::video::sprite_draw&,
		d2d::video::sprite_fill_draw&,
		d2d::video::animation_sprite_finder&,
		ldtools::ttf_manager&,
		const ldv::resource_manager&,
		const appenv::env&
	);
	                            ~game_draw();

	void                        draw(ldv::screen&, const app::map&, const app::player&);
	void                        draw_area_name_banner(ldv::screen&);
	void                        draw_lives_banner(ldv::screen&);

	void                        setup_area_name_banner(const std::string&);
	void                        setup_lives_banner(int);

	private:

	ldv::camera&                camera;
	d2d::video::scenery_tile_draw&  scenery_tile_draw;
	d2d::video::sprite_draw&        sprite_draw;
	d2d::video::sprite_fill_draw&   sprite_fill_draw;
	d2d::video::animation_sprite_finder& animation_sprite_finder;

	ldtools::view_composer      area_name_view;
	ldtools::view_composer      lives_banner_view;

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
	void                        draw_projectile_vertical(ldv::screen&, const app::projectile&);
	void                        draw_projectile_directed(ldv::screen&, const app::projectile&);
	void                        draw_projectile_falling(ldv::screen&, const app::projectile&);
	void                        draw_breaking_platform(ldv::screen&, const app::breaking_platform&);
	void                        draw_platform(ldv::screen&, const app::platform_block&);
	void                        draw_moving_block(ldv::screen&, const app::moving_block&);
	void                        draw_facing_block(ldv::screen&, const app::facing_block&);
};
}
