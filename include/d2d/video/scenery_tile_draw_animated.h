#pragma once

#include "scenery_tile_draw.h"
#include <ldtools/animation_table.h>
#include <functional>

namespace d2d { namespace video {

/**
 * A specialized class to draw animated tiles that still has the ability to 
 * draw regular tiles. To make use of this class two functions must be 
 * injected, one to tell if a tile is animated and another to tell the
 * animation that corresponds to a tile index.
 * It has full ownership of the scenery_tile_draw but NOT of the texture and
 * sprite table it contains!.
 */
class scenery_tile_draw_animated {

	//TODO: A bit... meh. The should be the counterpart of the animation_sprite_finder thingy.
	//but it is actually some sort of a mix... Ok, I get it now... There is
	//no reason NOT to have the animation_sprite_finder thing to retrieve
	//shit but... This is a bit more complicated, right???

	public:
	                                scenery_tile_draw_animated(
		const ldtools::sprite_table&, 
		const ldv::texture&, 
		int, 
		int, 
		const ldtools::animation_table&,
		ldv::camera * =nullptr,
		bool=false
	);

	//100% public, for sure.
	scenery_tile_draw               static_draw;

/**
 * the timer must be fed from the outside.
 */
	void                            tic(float);

/**
 * sets if a camera will be used.
 */
	scenery_tile_draw_animated&     set_with_camera(bool _value) {static_draw.set_with_camera(_value); return *this;}
	scenery_tile_draw_animated&     set_camera(ldv::camera& _camera) {static_draw.set_camera(_camera); return *this;}
	scenery_tile_draw_animated&     unset_camera() {static_draw.unset_camera(); return *this;}

/**
 * replaces the current animation table.
 */
	scenery_tile_draw_animated&     reset_animation(const ldtools::animation_table&);

/**
 * set a function to tell if a tile index is animated. The function will get
 * the tile index and must return true if that tile is animated.
 */
	scenery_tile_draw_animated&     set_is_animation_fn(std::function<bool(int)>);

/**
 * set a function to return the animation index of an animated tile index. The
 * function will get the tile index and must return the animation index. 
 */
	scenery_tile_draw_animated&     set_index_to_animation_fn(std::function<int(int)>);

/**
 * These methods use the internal timer.
 */
	void            draw(ldv::screen&, const scenery_tile&);

/**
 * draw versions to be called with an outside timer.
 */
	void            draw(ldv::screen&, const scenery_tile&, float);


/**
 * templated methods of the previous ones for ranged containers.
 */
	template<typename T>
	void            draw(ldv::screen& _screen, const T& _container) {

		for(const auto& tile : _container) {

			draw(_screen, tile);
		}
	}

	template<typename T>
	void            draw(ldv::screen& _screen, const T& _container, float _timer) {

		for(const auto& tile : _container) {

			draw(_screen, tile, _timer);
		}
	}

	private:

	const ldtools::animation_table * animation_table{nullptr};
	std::function<bool(int)>        index_is_animation{nullptr};
	std::function<int(int)>         index_to_animation{nullptr};
	float                           internal_timer{0.f};
};
}}
