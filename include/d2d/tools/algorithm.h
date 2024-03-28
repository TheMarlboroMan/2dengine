#pragma once

#include <algorithm>

namespace d2d { namespace tools {

/** wrappers for algorithms in complete containers.*/
template<typename c, typename f>
void for_each(
	c& _container,
	f _func
) {

	std::for_each(
		std::begin(_container),
		std::end(_container),
		_func
	);
}
}}
