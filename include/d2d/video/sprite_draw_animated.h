#pragma once

#include "sprite_draw.h"
#include <ldtools/animation_table.h>

namespace d2d { namespace video {

/*
 * TODO: What a terrible name xD!
 */
class sprite_draw_animated {

	public:

	                                sprite_draw_animated(
										const ldtools::animation_table&
									);

/**
 * the timer must be fed from the outside.
 */
	void                            tic(float);

/**
 * replaces the current animation table.
 */
	sprite_draw_animated&           reset_animation(const ldtools::animation_table&);

/**
 * gets an animation by index.
 */
	const ldtools::animation&      animation(int _index) const {return animation_table->get(_index);}

/**
 * get frame line from animation using the internal timer.
 */
	const ldtools::animation_line& get(const ldtools::animation&) const;
	const ldtools::animation_line& get(int) const;
/**
 * get frame line from animation using the given float time.
 */
	const ldtools::animation_line& get(const ldtools::animation&, float) const;
	const ldtools::animation_line& get(int, float) const;

/**
 * get frame line from animation using the given float time if the animation
 * lasted that other float time.
 */
	const ldtools::animation_line& get(const ldtools::animation&, float, float) const;
	const ldtools::animation_line& get(int, float, float) const;

/**
 * get the flags for the given line. The second version uses the original
 * flags as the point of starting.
 */
	sprite_draw::flags             flags(const ldtools::animation_line&) const;
	sprite_draw::flags             flags(const ldtools::animation_line&, sprite_draw::flags) const;

	private:

	const ldtools::animation_table * animation_table{nullptr};
	float                           internal_timer{0.f};
};
}}
