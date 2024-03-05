#include "app/button.h"
#include "app/definitions.h"

using namespace app;

button::button(
	d2d::collision::point _pt,
	types _type,
	int _id,
	int _tag,
	bool _used
):
	ent{_pt, app::tile_w, app::tile_h},
	type{_type},
	id{_id},
	tag{_tag},
	used{_used}
{}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const button& _button
) {

	_stream<<"button["<<_button.ent<<" id:"<<_button.id<<" tag:"<<_button.tag<<"]";
	return _stream;
}

