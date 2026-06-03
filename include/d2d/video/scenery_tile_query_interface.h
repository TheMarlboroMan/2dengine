#pragma once

namespace d2d { namespace video {

/**
 * represents a class that is able to answer two questions: does this tile
 * index represent an animation? and what animation index does this tile
 * represent?
 */
class scenery_tile_query_interface {

	public:

/**
 * must return true if the given index is an animation.
 */
	virtual bool    is_animation(int) const=0;
/**
 * must return the index of the animation the given index refers to. should
 * throw if the given index does not represent an animation.
 */
	virtual int     get_animation(int) const=0;
};

}}
