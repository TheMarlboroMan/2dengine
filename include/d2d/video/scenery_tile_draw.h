#pragma once

#include "sprite_draw.h"
#include "scenery_tile.h"

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
	                        int
	);

	//100% public, can be manipulated.
	d2d::video::sprite_draw     sprite_draw;

	void            draw(ldv::screen&, const ldv::camera&, const scenery_tile&);
	void            draw(ldv::screen&, const scenery_tile&);

/**
 * templated methods of the previous ones for ranged containers.
 */
	template<typename T>
	void            draw(ldv::screen& _screen, const ldv::camera& _camera, const T& _container) {

		for(const auto& tile : _container) {

			draw(_screen, _camera, tile);
		}
	}

	template<typename T>
	void            draw(ldv::screen& _screen, const T& _container) {

		for(const auto& tile : _container) {

			draw(_screen, tile);
		}
	}

	int             get_tile_w() const {return tile_w;}
	int             get_tile_h() const {return tile_h;}

	private:

	int                             tile_w{0},
	                                tile_h{0};
};
}}

