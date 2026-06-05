#pragma once

#include <string>
#include <tools/i8n.h>

namespace app {

/**
 * Simple service class shared between the main controller and the text one,
 * so that one can tell the other what text to display. 
 */
class show_text_exchange {

	public:

	enum colours {
		white=0,
		orange=1,
		red=2,
		blue=3,
		green=4
	};

	            show_text_exchange(const tools::i8n&);
	//!Sets the current key.
	show_text_exchange&     set(const std::string&);
	//!Returns the text corresponding to the current key.
	std::string             get() const;
	//!Sets the colour using any of the colour enums.
	show_text_exchange&     set_colour(int);
	//!Gets the colour,
	int                     get_colour() const {return colour;}
	//!Sets the keys for the answers.
	show_text_exchange&     set_answers(const std::string&, const std::string&, const std::string&);
	//!Removes all answers.
	show_text_exchange&     clear_answers();
	//!Returns true if answers have been set.
	bool                    has_answers() const;
	//!Set "key" as any of the third answers and clear them.
	show_text_exchange&     answer_first();
	show_text_exchange&     answer_second();
	show_text_exchange&     answer_third();

	private:

	std::string         key;
	std::string         first_answer;
	std::string         second_answer;
	std::string         third_answer;
	const tools::i8n&   i8n;
	int                 colour{0};

};
}
