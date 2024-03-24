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
 * It has full ownership of the sprite_draw but NOT of the texture and
 * sprite table it contains!.
 */
class scenery_tile_draw_animated {

	//TODO: A bit... meh. The should be the counterpart of the sprite_draw_animated thingy.

	public:
	                                scenery_tile_draw_animated(
		const ldtools::sprite_table&, const ldv::texture&, int, int, 
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
