#pragma once

#include <tools/number_generator.h>

namespace app {

/**
 * pseudo-random generator between 0-255. A instance so that we can add doom
 * style pseudo-random deterministic values.
 */
class random {

	public:

	        random();
	//Returns a number between min and max (the two args) assuming that they
	//are in the range 0-255. The values are inclusive.
	int     get(int, int);

	private:

	tools::int_generator numgen;
};

}
