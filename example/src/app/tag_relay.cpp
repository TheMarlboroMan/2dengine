#include "app/tag_relay.h"

using namespace app;

std::ostream& app::operator<<(
	std::ostream& _stream,
	const tag_relay& _node
) {

	_stream<<"tag_relay["<<_node.tag<<"]";
	return _stream;
}
