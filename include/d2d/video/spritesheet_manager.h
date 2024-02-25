#pragma once

#include <ldtools/sprite_table.h>
#include <map>

namespace d2d { namespace video {

/**
* very simple manager for sprite sheet tables.
*/
class spritesheet_manager {

	public:

/**
 * returns the sheet at the given index. Throws if cannot be found.
 */
	const ldtools::sprite_table&            at(int _index) const;

/**
 * adds a new sprite table. Will throw if attempting to replace an existing one.
 */
	spritesheet_manager&                    add(int, const ldtools::sprite_table&);

	private: 

	std::map<int, ldtools::sprite_table>    container;
};

}}

