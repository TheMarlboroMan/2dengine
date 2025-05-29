#include <app/moving_block_node.h>

using namespace app;

moving_block_node::moving_block_node(
	d2d::collision::point _pt,
	int _id,
	int _next_id,
	int _velocity_to_next,
	int _wait_here_ms
):
	point{_pt},
	id{_id},
	nextid{_next_id},
	velocity{_velocity_to_next},
	wait_ms{_wait_here_ms}
{}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const moving_block_node& _node
) {

	_stream<<"moving_block_node["<<_node.point<<"]";
	return _stream;
}
