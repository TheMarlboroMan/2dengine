#include "app/button.h"
#include "app/definitions.h"

using namespace app;

button::button(
	d2d::collision::point _pt,
	int _type,
	int _tag
):
	ent{_pt, app::tile_w, app::tile_h},
	type{_type},
	tag{_tag}
{}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const button& _button
) {

	_stream<<"button["<<_button.ent<<"]";
	return _stream;
}

