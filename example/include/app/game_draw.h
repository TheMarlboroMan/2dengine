#pragma once

#include "app/env.h"
#include "app/random.h"
#include "app/map_transition.h"
#include "app/background_interface.h"
#include <d2d/components/particle.h>
#include <d2d/video/scenery_tile_draw.h>
#include <d2d/video/sprite_draw.h>
#include <d2d/video/sprite_fill_draw.h>
#include <d2d/video/animation_sprite_finder.h>
#include <ldtools/ttf_manager.h>
#include <ldtools/view_composer.h>
#include <ldv/screen.h>
#include <ldv/camera.h>
#include <ldv/ttf_representation.h>
#include <ldv/ttf_font.h>
#include <ldv/box_representation.h>
#include <ldv/bitmap_representation.h>
#include <ldv/resource_manager.h>
#include <string>
#include <memory>

namespace app {

using d2d::components::particle_index;
using d2d::components::particle;

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
class toggle_block;
class exit;
class moving_block;
class boss;
class boss_skull;

/**
 * business class to draw the main game state.
 * It implements the particle_render_interface because I am lazy and I 
 * want to use the tools in this class to draw particles with the less
 * possible hassle.
 */
class game_draw:
	public d2d::components::particle_render_interface {

	public:

	                            game_draw(
		ldv::camera&,
		d2d::video::scenery_tile_draw&,
		d2d::video::sprite_draw&,
		d2d::video::sprite_fill_draw&,
		d2d::video::animation_sprite_finder&,
		ldtools::ttf_manager&,
		const ldv::resource_manager&,
		const appenv::env&,
		random&
	);
	                            ~game_draw();

	void                        draw(ldv::screen&, const app::map&, const app::player&, int discovered_rooms, const app::map_transition*);
	void                        draw_area_name_banner(ldv::screen&);
	void                        draw_lives_banner(ldv::screen&);

	void                        setup_area_name_banner(const std::string&);
	void                        setup_lives_banner(int);

	//!Can be used to set a background or null.
	void                        set_background(background_interface * _bg) {background=_bg;}

	//////
	//implementation of particle_render_interface
	bool must_subscribe(const particle&) const;
	void subscribe(const particle&, particle_index);
	void expire(const particle&, particle_index);
	void draw(const d2d::components::particle&, ldv::screen&, particle_index);
	void draw(const d2d::components::particle& _particle, ldv::screen& _screen, const ldv::camera&, particle_index _index) {draw(_particle, _screen, _index);}
	//end of implementation of particle_render_interface
	/////

	private:

	//Outside references..
	ldv::camera&                camera;
	d2d::video::scenery_tile_draw&  scenery_tile_draw;
	d2d::video::sprite_draw&        sprite_draw;
	d2d::video::sprite_fill_draw&   sprite_fill_draw;
	d2d::video::animation_sprite_finder& animation_sprite_finder;
	random&                     rng;

	//Properties...
	ldtools::view_composer      area_name_view;
	ldtools::view_composer      lives_banner_view;
	ldv::ttf_font               exit_number_font;
	//A table of N integers to store the "drawing types" of particles.
	std::vector<int>            particle_persistence_table;
	background_interface *      background{nullptr};

	void                        draw_player(ldv::screen&, const app::player&);
	void                        draw_ladder(ldv::screen&, const app::ladder&);
	void                        draw_collectible(ldv::screen&, const app::collectible&);
	void                        draw_linear_monster(ldv::screen&, const app::linear_monster&);
	void                        draw_leaping_monster(ldv::screen&, const app::leaping_monster&);
	void                        draw_timed_trap(ldv::screen&, const app::timed_trap&);
	void                        draw_secret_cover(ldv::screen&, const app::secret_cover&);
	void                        draw_button(ldv::screen&, const app::button&);
	void                        draw_one_use_button(ldv::screen&, const app::button&);
	void                        draw_repeatable_button(ldv::screen&, const app::button&);
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
	void                        draw_exit(ldv::screen&, const app::exit&, int);
	void                        draw_toggle_block(ldv::screen&, const app::toggle_block&);
	void                        draw_boss(ldv::screen&, const app::boss&);
	void                        draw_boss_regular(ldv::screen&, const app::boss&);
	void                        draw_boss_defeat(ldv::screen&, const app::boss&);
	void                        draw_boss_skull(ldv::screen&, const app::boss_skull&);
	void                        draw_animated_particle(ldv::screen&, const d2d::components::particle&, int, int);
	void                        draw_indexed_sprite_particle(ldv::screen&, const d2d::components::particle&, particle_index, int);
	void                        draw_background_somewhere_else(ldv::screen&);
};
}
