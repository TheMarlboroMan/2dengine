#pragma once

#include <ldtools/animation_table.h>
#include <map>

namespace d2d { namespace video {

/**
* very simple manager for animation sheet tables.
*/
class animation_manager {

	public:

/**
 * returns the animation tableat the given index. Throws if cannot be found.
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

