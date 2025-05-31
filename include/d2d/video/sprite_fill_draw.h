#pragma once

#include <ldv/screen.h>
#include <ldv/camera.h>
#include <ldv/texture.h>
#include <ldv/bitmap_representation.h>
#include <ldtools/sprite_table.h>

namespace d2d { namespace video {

/**
 * shares a lot of DNA with sprite_draw, the whole setters and camera and stuff.
 */
class sprite_fill_draw {

	public:


	                                sprite_fill_draw(const ldtools::sprite_table&, const ldv::texture&, ldv::camera* =nullptr, bool=false);

/**
 * in this screen, fill the rectangle with the given frame.
 */
	void                            fill(ldv::screen&, ldv::rect, const ldtools::sprite_frame&);

/**
 * in this screen, fill the rectangle with the frame identified by the given integer.
 */
	void                            fill(ldv::screen&, ldv::rect, int);

/**
 * sets if a camera will be used.
 */
	sprite_fill_draw&               set_with_camera(bool);

/**
 * sets a camera. if not null a with_camera is true the camera will be used.
 * the camera must outlive this instance.
 */
	sprite_fill_draw&               set_camera(ldv::camera&);

/**
 * removes the camera.
 */

	sprite_fill_draw&               unset_camera();

/**
 * replaces the current sprite table
 */
	sprite_fill_draw&               reset_sprite_table(const ldtools::sprite_table&);

/**
 * replaces the current texture.
 */
	sprite_fill_draw&               reset_texture(const ldv::texture&);

	private:

	ldv::bitmap_representation      bmp;
	const ldtools::sprite_table *   sprite_table{nullptr};
	ldv::camera *                   camera{nullptr};
	bool                            with_camera{false};
};
}}
