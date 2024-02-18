#pragma once

#include <functional>
#include <algorithm>

namespace d2d { namespace collision {

/**
 * returns a vector of tiles (references or pointers) for those the predicate
 * _fn returns true. Specializations for vectors of pointers and references
 * are provided.
 */
template <typename T> 
std::vector<const T*>          filter_tiles(
	const std::vector<const T*>& _tiles, 
	std::function<bool(const T*)>& _fn
) {

	std::vector<const T*> result{};
	std::copy_if(
		std::begin(_tiles),
		std::end(_tiles),
		std::back_inserter(result),
		_fn
	);

	return result;
}

template <typename T>
std::vector<const T>          filter_tiles(
	const std::vector<const T>& _tiles, 
	std::function<bool(const T&)>& _fn
) {

	std::vector<const T> result{};
	std::copy_if(
		std::begin(_tiles),
		std::end(_tiles),
		std::back_inserter(result),
		_fn
	);

	return result;
}
}}
