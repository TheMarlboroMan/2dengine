#pragma once

#include <string>
#include <iostream>

namespace app {

//Element that exists in the map with no position and that triggers the 
//apparition of the show_text controller with the given text. If the "answer"
//indexes are given, this text will trigger the question controller when done,
//which will use the chosen index as the next text to show.
class text_node {

	public:

	int             event_id,
	                tag;
	std::string     text_index,
	                first_answer_index,
	                second_answer_index,
	                third_answer_index;
};

std::ostream& operator<<(std::ostream&, const text_node&);
}
