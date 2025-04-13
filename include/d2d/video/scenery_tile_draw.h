#pragma once

#include "sprite_draw.h"
#include "scenery_tile.h"

namespace d2d { namespace video {

/**
 * specialized class to draw scenery tiles. It owns the sprite draw but does
 * not own the texture and sprite_table it contains. This class may be used
 * when the tiles are static and there are no animations. If animations are
 * to be used, scenery_tile_draw_animated is a better bet.
 */
class scenery_tile_draw{

	public:

	                    scenery_tile_draw(
	                        const ldtools::sprite_table&, 
	                        const ldv::texture&, 
	                        int, 
	                        int,
	                        ldv::camera * = nullptr,
	                        bool=false
	);

	//100% public, can be manipulated.
	d2d::video::sprite_draw     sprite_draw;

	void            draw(ldv::screen&, const scenery_tile&);

/**
 * templated methods of the previous ones for ranged containers.
 */
	template<typename T>
	void            draw(ldv::screen& _screen, const T& _container) {

		for(const auto& tile : _container) {

			draw(_screen, tile);
		}
	}

	int             get_tile_w() const {return tile_w;}
	int             get_tile_h() const {return tile_h;}

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
};
}}

