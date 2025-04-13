#pragma once

#include <ldtools/animation_table.h>
#include <map>

namespace d2d { namespace video {

/**
* very simple manager for animation sheet tables. Just a reminder, an
* animation table contains animations (identified by an index), which in
* turn contains lines (identified by their index, containing the frame) so
* this class allows us to have many animation tables, each identified by their
* own index.
*/
class animation_manager {

	public:

/**
 * returns the animation table at the given index. Throws if cannot be found.
 */
	const ldtools::animation_table&         at(int _index) const;

/**
 * adds a new table. Will throw if attempting to replace an existing one.
 */
	animation_manager&                      add(int, const ldtools::animation_table&);

	private: 

	std::map<int, ldtools::animation_table>    container;
};

}}

