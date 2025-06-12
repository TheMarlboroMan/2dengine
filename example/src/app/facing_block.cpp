#include "app/facing_block.h"

using namespace app;

facing_block::facing_block(
	const d2d::collision::box& _box,
	faces _face,
	int _type
) 
	:ent{_box},
	type{_type},
	face{_face}
{}

void facing_block::sync(
	faces _face
) {

	active=face==_face;
}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const facing_block& _block
) {

	_stream<<"facing_block["<<_block.ent<<"]";
	return _stream;
}

