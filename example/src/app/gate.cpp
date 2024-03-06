#include "app/gate.h"
#include "app/definitions.h"

using namespace app;

gate::gate(
	d2d::collision::box _box,
	int _id,
	int _tag
):
	ent{_box},
	id{_id},
	tag{_tag}
{}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const gate& _gate
) {

	_stream<<"gate["<<_gate.ent<<" id:"<<_gate.id<<" tag:"<<_gate.tag<<"]";
	return _stream;
}

