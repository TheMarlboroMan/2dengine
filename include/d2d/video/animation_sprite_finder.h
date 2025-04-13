#pragma once

#include "sprite_draw.h"
#include <ldtools/animation_table.h>

namespace d2d { namespace video {

/*
 * State-aware class. Must be fed with an animation table and it allows
 * the retrieval of animations and frames according to an internal timer or
 * to external information. It also allows for the calculation of sprite
 * modifiers as expressed by sprite_draw. The animation table can be replaced 
 * at runtime.
 */
class animation_sprite_finder {

	public:

	                               animation_sprite_finder(const ldtools::animation_table&);

/**
 * the timer must be fed from the outside.
 */
	void                           tic(float);

/**
 * replaces the current animation table.
 */
	animation_sprite_finder&       reset_table(const ldtools::animation_table&);

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
 * flags as the starting point.
 */
	sprite_draw::modifiers         modifiers(const ldtools::animation_line&) const;
	sprite_draw::modifiers         modifiers(const ldtools::animation_line&, sprite_draw::modifiers) const;

	private:

	const ldtools::animation_table * animation_table{nullptr};
	float                            internal_timer{0.f};
};
}}
