#include "app/text_node.h"

using namespace app;

std::ostream& app::operator<<(
	std::ostream& _stream,
	const text_node& _node
) {

	_stream<<"text_node["<<_node.text_index<<"]";
	return _stream;
}
