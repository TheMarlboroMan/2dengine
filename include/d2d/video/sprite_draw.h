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

/**
 * these can be passed to draw methods that accept it to modify the behaviour
 * of the drawing algorithm.
 */
	struct flags {
		bool    flip_horizontal{false},
		        flip_vertical{false};
		int     rotation_degrees{0};
	};

	                                sprite_draw(const ldtools::sprite_table&, const ldv::texture&);

//TODO: The fact that these use the frame flags could be problematic if we
//later want to use additional effects - like colorizing - without discarding
//the frame info.
 
/**
 * draw sprites by sprite or by index in the given point / rect, with flags
 * or using the default sprite flags.
 */
	void                            draw(ldv::screen&, const ldv::camera&, ldv::point, const ldtools::sprite_frame&, flags);
	void                            draw(ldv::screen&, const ldv::camera&, ldv::point, const ldtools::sprite_frame&);
	void                            draw(ldv::screen&, const ldv::camera&, ldv::point, int, flags);
	void                            draw(ldv::screen&, const ldv::camera&, ldv::point, int);
	void                            draw(ldv::screen&, const ldv::camera&, ldv::rect, const ldtools::sprite_frame&, flags);
	void                            draw(ldv::screen&, const ldv::camera&, ldv::rect, const ldtools::sprite_frame&);
	void                            draw(ldv::screen&, const ldv::camera&, ldv::rect, int, flags);
	void                            draw(ldv::screen&, const ldv::camera&, ldv::rect, int);

/** same as above, no camera */
	void                            draw(ldv::screen&, ldv::point, const ldtools::sprite_frame&, flags);
	void                            draw(ldv::screen&, ldv::point, const ldtools::sprite_frame&);
	void                            draw(ldv::screen&, ldv::point, int, flags);
	void                            draw(ldv::screen&, ldv::point, int);
	void                            draw(ldv::screen&, ldv::rect, const ldtools::sprite_frame&, flags);
	void                            draw(ldv::screen&, ldv::rect, const ldtools::sprite_frame&);
	void                            draw(ldv::screen&, ldv::rect, int, flags);
	void                            draw(ldv::screen&, ldv::rect, int);

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

