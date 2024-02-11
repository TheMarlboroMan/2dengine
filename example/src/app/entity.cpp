#include "app/entity.h"

using namespace app;

entity::entity(
	int _x,
	int _y
):
	collision_box{{(double)_x, (double)_y}, 8, 24}
{}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const entity& _entity
) {

	const auto& box=_entity.get_box();
	_stream<<box.origin.x<<","<<box.origin.y<<" "<<box.w<<"x"<<box.h;
	return _stream;
}
