#pragma once

#include <ldv/screen.h>
#include <ldv/camera.h>
#include <d2d/video/scenery_tile_draw_animated.h>
#include <d2d/video/sprite_draw.h>
#include <d2d/video/sprite_draw_animated.h>

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
		ldv::screen&, 
		const ldv::camera&,
		d2d::video::scenery_tile_draw_animated&, 
		d2d::video::sprite_draw&,
		d2d::video::sprite_draw_animated&
	);

	void                        draw(const app::map&, const app::player&);

	private:

	ldv::screen&                screen;
	const ldv::camera&          camera;
	d2d::video::scenery_tile_draw_animated&  scenery_tile_draw;
	d2d::video::sprite_draw&        sprite_draw;
	d2d::video::sprite_draw_animated& sprite_draw_animated;

	void                        draw_player(const app::player&);
	void                        draw_ladder(const app::ladder&);
	void                        draw_collectible(const app::collectible&);
	void                        draw_linear_monster(const app::linear_monster&);
	void                        draw_leaping_monster(const app::leaping_monster&);
	void                        draw_timed_trap(const app::timed_trap&);
	void                        draw_secret_cover(const app::secret_cover&);
	void                        draw_button(const app::button&);
	void                        draw_gate(const app::gate&);
	void                        draw_projectile(const app::projectile&);
	void                        draw_projectile_linear(const app::projectile&);
	void                        draw_projectile_directed(const app::projectile&);
	void                        draw_breaking_platform(const app::breaking_platform&);
	void                        draw_platform(const app::platform_block&);
};
}
