#include "app/entry.h"

using namespace app;

entry::entry(
	d2d::collision::box _box,
	int _id, 
	int _position
):
	ent{_box},
	id{_id},
	position{_position}
{}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const entry& _entry
) {

	_stream<<"entry["<<_entry.ent.get_box()<<" id:"<<_entry.id<<" pos:"<<_entry.position;
	return _stream;
}

