#pragma once

#include <string>
#include <iostream>

namespace app {

//Element that exists in the map with no position and that triggers the 
//apparition of the show_text controller with the given text.
class text_node {

	public:

	int             event_id,
	                tag;
	std::string     text_index;
};

std::ostream& operator<<(std::ostream&, const text_node&);
}
