#include "app/autopilot_node.h"

using namespace app;

autopilot_node::autopilot_node(
	const d2d::collision::box& _box,
	int _input,
	int _pause_ms
):
	ent{_box},
	input{_input},
	pause{(double)_pause_ms / 1000.}
{}

void autopilot_node::use() {

	used=true;
}

void autopilot_node::reset() {

	used=false;
}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const autopilot_node& _node
) {

	_stream<<"autopilot_node["<<_node.ent<<" input:"<<_node.input<<"]";
	return _stream;
}
