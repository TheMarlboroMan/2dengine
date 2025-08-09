#include "app/boss_skull.h"

using namespace app;

boss_skull::boss_skull(
	d2d::collision::point _origin,
	double _init_timeout
):
	ent{_origin, w, h},
	timeout{_init_timeout, 0, false}
{}

void boss_skull::desintegrate() {

	to_be_destroyed=true;
}

void boss_skull::tic(
	ldtools::tdelta _delta
) {

	timeout.tic(_delta);
}

bool boss_skull::can_be_hit() const {

	return timeout.is_finished();
}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const boss_skull& _node
) {

	_stream<<"skull["<<_node.ent.get_origin()<<"]";
	return _stream;
}
