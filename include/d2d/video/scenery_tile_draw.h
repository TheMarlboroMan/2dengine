#pragma once

#include "scenery_tile.h"
#include <ldv/screen.h>
#include <ldv/camera.h>
#include <ldv/bitmap_representation.h>
#include <ldv/texture.h>
#include <ldtools/sprite_table.h>
#include <map>

namespace d2d { namespace video {

/**
 * specialized class to draw scenery tiles. It does NOT own the sprite table
 * or the bitmap it uses.
 */
class scenery_tile_draw {

	public:

	                    scenery_tile_draw(int, int, const ldtools::sprite_table&, const ldv::texture&);

/**
 * replaces the current sprite table
 */
	scenery_tile_draw&  reset_sprite_table(const ldtools::sprite_table&);

/**
 * replaces the current texture.
 */
	scenery_tile_draw&  reset_texture(const ldv::texture&);


/**
 * draws a tile to a screen accounting for the camera.
 */
	void            draw(ldv::screen&, const ldv::camera&, const scenery_tile&);

/**
 * draws a tile to a screen, no camera at all.
 */
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

	private:

	int                             tile_w{0},
	                                tile_h{0};
	ldv::bitmap_representation      bmp;
	const ldtools::sprite_table *   sprite_table{nullptr};
};
}}
