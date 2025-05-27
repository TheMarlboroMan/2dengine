#pragma once

#include "tile.h"
#include "spatiable.h"
#include "tile_finder.h"

#ifdef IS_DEBUG_BUILD
#include <lm/logger.h>
#endif
#include <vector>
#include <iostream>
#include <algorithm>
#include <initializer_list>

namespace d2d { namespace collision {

class tiles_in_box {

	public: 

	                    tiles_in_box(int, int);

#ifdef IS_DEBUG_BUILD

	tiles_in_box&       set_debug_enabled(bool _val) {debug_enabled=_val; return *this;}
	tiles_in_box&       set_logger(lm::logger* _val) {logger=_val; return *this;}
#endif

/**
 * Broad tile-based collision finder.
 * finds the tiles that are in contact with the given spatiable.
 * For a tile to be found it must exist (that is, when a tile has
 * nothing on it it does not exist at all). Does not check it the 
 * contents of the tile (which may be of a different size) actually
 * collide with the spatiable.
 */
	std::vector<const d2d::collision::tile *> find(
		const d2d::collision::spatiable&,
		const d2d::collision::tile_finder&
	) const;

	std::vector<const d2d::collision::tile *> find(
		const d2d::collision::box&,
		const d2d::collision::tile_finder&
	) const;

/**
 * Template based versions of the previous that also take a callable
 * argument that expects both const references to the spatiable's box and to 
 * the tile. For a tile to be included in the results it must be in
 * contact with the spatiable (as in the previous version) and also
 * pass this callable test by returning true.
 */
	template<typename C>
	std::vector<const d2d::collision::tile *> find(
		const d2d::collision::spatiable& _spatiable,
		const d2d::collision::tile_finder& _finder,
		C _callable
	) const {

		return find(_spatiable.get_box(), _finder, _callable);
	}

	template<typename C>
	std::vector<const d2d::collision::tile *> find(
		const d2d::collision::box& _box,
		const d2d::collision::tile_finder& _finder,
		C _callable
	) const {

		const auto& previous_results=find(_box, _finder);
		std::vector<const d2d::collision::tile *> result{};
		std::copy_if(
			std::begin(previous_results),
			std::end(previous_results),
			std::back_inserter(result),
			[&](const d2d::collision::tile * _tile) -> bool {
				return _callable(_box, *_tile);
			}
		);

		return result;
	}

/**
 * same as before but with a list of callables! Test on all callables
 * must return true and should be all of the same type, so fiddling is required.
 */

	template<typename C>
	std::vector<const d2d::collision::tile *> find(
		const d2d::collision::spatiable& _spatiable,
		const d2d::collision::tile_finder& _finder,
		std::initializer_list<C> _callables
	) const {

		return find(_spatiable.get_box(), _finder, _callables);
	}

	template<typename C>
	std::vector<const d2d::collision::tile *> find(
		const d2d::collision::box& _box,
		const d2d::collision::tile_finder& _finder,
		std::initializer_list<C> _callables
	) const {

		const auto& previous_results=find(_box, _finder);
		std::vector<const d2d::collision::tile *> result{};
		std::copy_if(
			std::begin(previous_results),
			std::end(previous_results),
			std::back_inserter(result),
			[&](const d2d::collision::tile * _tile) -> bool {

				for(const auto& callable : _callables) {

					if(!callable(_box, *_tile)) {

						return false;
					}
				}

				return true;
			}
		);

		return result;
	}

/**
 * early returning version of find that just checks if a collision exists.
 */
	template<typename C>
	bool has(
		const d2d::collision::spatiable& _spatiable,
		const d2d::collision::tile_finder& _finder,
		C _callable
	) const {

		return this->has(_spatiable.get_box(), _finder, _callable);
	}

	template<typename C>
	bool has(
		const d2d::collision::box& _box,
		const d2d::collision::tile_finder& _finder,
		C _callable
	) const {

		const auto& previous_results=find(_box, _finder);
		for(const auto& _node : find(_box, _finder)) {

			if(_callable(_box, *_node)) {

				return true;
			}
		}

		return false;
	}

	template<typename C>
	bool has(
		const d2d::collision::spatiable& _spatiable,
		const d2d::collision::tile_finder& _finder,
		std::initializer_list<C> _callables
	) const {

		return this->has(_spatiable.get_box(), _finder, _callables);
	}

	template<typename C>
	bool has(
		const d2d::collision::box& _box,
		const d2d::collision::tile_finder& _finder,
		std::initializer_list<C> _callables
	) const {

		const auto& previous_results=find(_box, _finder);
		for(const auto& _node : find(_box, _finder)) {

			for(const auto& callable : _callables) {

				if(!callable(_box, *_node)) {

					return false;
				}
			}

			return true;
		}

		return false;
	}

	private:

	int                 tile_w,
						tile_h;
#ifdef IS_DEBUG_BUILD
	lm::logger*         logger;
	bool                debug_enabled{false};
#endif
};
}}
