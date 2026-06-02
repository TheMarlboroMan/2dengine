#pragma once

#include <string>
#include <iostream>
#include <vector>

namespace app {

/**
* element that exists in the map with no position and that is able to trigger
* another tag when its own has been triggered.
*/

class tag_relay {

	public:

	int             tag;
	std::vector<int> relays;
};

std::ostream& operator<<(std::ostream&, const tag_relay&);
}
