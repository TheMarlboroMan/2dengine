#pragma once

#include "sprite_draw.h"
#include "scenery_tile.h"

#include <ldtools/animation_table.h>
#include <functional>

namespace d2d { namespace video {

/**
 * specialized class to draw scenery tiles. It owns the sprite draw but does
 * not own the texture and sprite_table it contains. 
 */
class scenery_tile_draw{

	public:

	                    scenery_tile_draw(
	                        const ldtools::sprite_table&, 
	                        const ldv::texture&, 
	                        int, 
	                        int,
	                        const ldtools::animation_table * = nullptr,
	                        ldv::camera * = nullptr,
	                        bool=false
	);

	//100% public, can be manipulated.
	d2d::video::sprite_draw     sprite_draw;

/**
 * simple draw
 */
	void            draw(ldv::screen&, const scenery_tile&);

/**
 * templated method of the previous one for ranged containers.
 */
	template<typename T>
	void            draw(ldv::screen& _screen, const T& _container) {

		for(const auto& tile : _container) {

			draw(_screen, tile);
		}
	}

/**
 * This method uses the internal timer.
 */
	void            draw_animation(ldv::screen&, const scenery_tile&);

/**
 * version to be called with an outside timer.
 */
	void            draw_animation(ldv::screen&, const scenery_tile&, float);

/**
 * templated methods of the previous ones for ranged containers.
 */
	template<typename T>
	void            draw_animation(ldv::screen& _screen, const T& _container) {

		for(const auto& tile : _container) {

			draw_animation(_screen, tile);
		}
	}

	template<typename T>
	void            draw_animation(ldv::screen& _screen, const T& _container, float _timer) {

		for(const auto& tile : _container) {

			draw_animation(_screen, tile, _timer);
		}
	}

	int             get_tile_w() const {return tile_w;}
	int             get_tile_h() const {return tile_h;}

/**
 * the timer must be fed from the outside.
 */
	void                            tic(float);

/**
 * replaces the current animation table.
 */
	scenery_tile_draw&     reset_animation_table(const ldtools::animation_table&);

/**
 * sets the method that matches a tile index to its animation
 */
	scenery_tile_draw&     set_index_to_animation_fn(std::function<int(int)>);

/**
 * sets the method that identifies a tile index as animated.
 */
	scenery_tile_draw&     set_is_animation_fn(std::function<bool(int)>);

/**
 * sets if a camera will be used.
 */
	scenery_tile_draw&  set_with_camera(bool _value) {sprite_draw.set_with_camera(_value); return *this;}

/**
 * sets a camera. If not null a with_camera is true the camera will be used.
 * The camera must outlive this instance.
 */
	scenery_tile_draw&  set_camera(ldv::camera& _camera) {sprite_draw.set_camera(_camera); return *this;}

/**
 * removes the camera.
 */

	scenery_tile_draw&  unset_camera() {sprite_draw.unset_camera(); return *this;}

	private:

	int                             tile_w{0},
	                                tile_h{0};

	const ldtools::animation_table * animation_table{nullptr};
	std::function<bool(int)>        index_is_animation{nullptr};
	std::function<int(int)>         index_to_animation{nullptr};
	float                           internal_timer{0.f};
};
}}

