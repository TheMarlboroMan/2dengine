#include "app/boss_skull.h"

using namespace app;

boss_skull::boss_skull(
	d2d::collision::point _origin
):
	ent{_origin, w, h}
{}

void boss_skull::desintegrate() {

	to_be_destroyed=true;
}


std::ostream& app::operator<<(
	std::ostream& _stream,
	const boss_skull& _node
) {

	_stream<<"skull["<<_node.ent.get_origin()<<"]";
	return _stream;
}
