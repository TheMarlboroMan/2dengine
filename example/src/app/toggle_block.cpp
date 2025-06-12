#include "app/toggle_block.h"

using namespace app;


toggle_block::toggle_block(
	const d2d::collision::box& _box,
	bool _active,
	int _type
): 
	ent{_box},
	active{_active},
	initial_active{active},
	type{_type}
{}

void toggle_block::reset() {

	active=initial_active;
}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const toggle_block& _block
) {

	_stream<<"toggle_block["<<_block.ent<<"]";
	return _stream;
}

