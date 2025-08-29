#include "app/show_text_exchange.h"

using namespace app;

show_text_exchange::show_text_exchange(
	const tools::i8n& _i8n
):i8n{_i8n}
{}

void show_text_exchange::set(
	const std::string& _key
) {

	key=_key;

}

std::string show_text_exchange::get() const {

	return i8n.get(key);
}
