#pragma once

#include <ldv/screen.h>
#include <ldv/camera.h>
#include <ldv/texture.h>
#include <ldv/bitmap_representation.h>
#include <ldtools/sprite_table.h>

namespace d2d { namespace video {

/**
 * sprite draw class. Does not own the texture or sprite table it uses.
 */
class sprite_draw {

	public:

	                                sprite_draw(const ldtools::sprite_table&, const ldv::texture&);

/**
 * draws a sprite in the given world point.
 */
	void                            draw(ldv::screen&, const ldv::camera&, ldv::point, const ldtools::sprite_frame&);
	void                            draw(ldv::screen&, ldv::point, const ldtools::sprite_frame&);
/**
 * draws a sprite by index in the given world point.
 */
	void                            draw(ldv::screen&, const ldv::camera&, ldv::point, int);
	void                            draw(ldv::screen&, ldv::point, int);

/**
 * templated methods of the previous ones for ranged containers.
 */
	template<typename T>
	void            draw(ldv::screen& _screen, const ldv::camera& _camera, const T& _container) {

		for(const auto& sprite : _container) {

			draw(_screen, _camera, sprite);
		}
	}

	template<typename T>
	void            draw(ldv::screen& _screen, const T& _container) {

		for(const auto& sprite : _container) {

			draw(_screen, sprite);
		}
	}

/**
 * replaces the current sprite table
 */
	sprite_draw&                    reset_sprite_table(const ldtools::sprite_table&);

/**
 * replaces the current texture.
 */
	sprite_draw&                    reset_texture(const ldv::texture&);

	private:

	ldv::bitmap_representation      bmp;
	const ldtools::sprite_table *   sprite_table{nullptr};
};
}}

