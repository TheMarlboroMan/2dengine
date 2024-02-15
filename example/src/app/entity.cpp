#include "app/entity.h"

using namespace app;

entity::entity(
	int _x,
	int _y
):
	collision_box{{(double)_x, (double)_y}, 8, 24},
	previous_box{collision_box}
{}

void entity::sync_boxes() {

	previous_box=collision_box;
}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const entity& _entity
) {

	_stream<<_entity.get_box()<<" (prev "<<_entity.get_previous_box()<<")"<<std::endl;
	return _stream;
}
