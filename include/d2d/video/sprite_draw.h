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

