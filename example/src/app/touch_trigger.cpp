#include "app/touch_trigger.h"

using namespace app;

touch_trigger::touch_trigger(
	const d2d::collision::box& _box,
	int _id,
	int _tag,
	bool _used
):
	ent{_box},
	id{_id},
	tag{_tag},
	used{_used}
{}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const touch_trigger& _trigger
) {

	_stream<<"touch_trigger["<<_trigger.ent<<" id:"<<_trigger.id<<" tag:"<<_trigger.tag<<"]";
	return _stream;
}
