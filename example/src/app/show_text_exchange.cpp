#include "app/show_text_exchange.h"

using namespace app;

show_text_exchange::show_text_exchange(
	const tools::i8n& _i8n
):i8n{_i8n}
{}

show_text_exchange& show_text_exchange::set(
	const std::string& _key
) {

	key=_key;
	return *this;
}

show_text_exchange& show_text_exchange::clear_answers() {

	first_answer.clear();
	second_answer.clear();
	third_answer.clear();
	return *this;
}

std::string show_text_exchange::get() const {

	return i8n.get(key);
}

bool show_text_exchange::has_answers() const {

	return 0!=first_answer.size();
}

show_text_exchange& show_text_exchange::set_answers(
	const std::string& _first,
	const std::string& _second,
	const std::string& _third
) {

	first_answer=_first;
	second_answer=_second;
	third_answer=_third;
	return *this;
}

show_text_exchange& show_text_exchange::answer_first() {

	key=first_answer;
	clear_answers();
	return *this;
}

show_text_exchange& show_text_exchange::answer_second() {

	key=second_answer;
	clear_answers();
	return *this;
}

show_text_exchange& show_text_exchange::answer_third() {

	key=third_answer;
	clear_answers();
	return *this;
}

show_text_exchange& show_text_exchange::set_colour(
	int _colour
) {

	colour=_colour;
	return *this;
}

