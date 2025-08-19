#include "app/button.h"
#include "app/definitions.h"

using namespace app;

button::button(
	d2d::collision::point _pt,
	types _type,
	int _id,
	int _tag,
	bool _used,
	bool _keep_used
):
	ent{_pt, app::tile_w, app::tile_h},
	type{_type},
	id{_id},
	tag{_tag},
	used{_used},
	keep_used_when_reset{_keep_used}
{}

void button::reset() {

	if(!keep_used_when_reset) {

		used=false;
	}
}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const button& _button
) {

	_stream<<"button["<<_button.ent<<" id:"<<_button.id<<" tag:"<<_button.tag<<"]";
	return _stream;
}

