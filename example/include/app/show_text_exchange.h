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

	            show_text_exchange(const tools::i8n&);
	//!Sets the current key.
	void        set(const std::string&);
	//!Returns the text corresponding to the current key.
	std::string get() const;

	private:

	std::string         key;
	const tools::i8n&   i8n;
};
}
