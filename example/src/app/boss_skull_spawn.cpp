#include <app/boss_skull_spawn.h>

using namespace app;

boss_skull_spawn::boss_skull_spawn(
	d2d::collision::point _pt,
	int _id
):
	point{_pt},
	id{_id}
{}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const boss_skull_spawn& _node
) {

	_stream<<"boss_skull_spawn["<<_node.point<<"]";
	return _stream;
}
