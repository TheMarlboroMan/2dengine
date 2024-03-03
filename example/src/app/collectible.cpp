#include "app/collectible.h"

using namespace app;

collectible::collectible(
	d2d::collision::point _pt,
	int _id,
	int _type
):
	id{_id},
	type{_type},
	ent{_pt.x, _pt.y, 8, 8}
{

	switch(type) {

		case gold_ingot:
			ent.set_w(gold_ingot_w);
			ent.set_h(gold_ingot_h);

		break;
		case gem:
			ent.set_w(gem_w);
			ent.set_h(gem_h);
		break;
		case ruby:
			ent.set_w(ruby_w);
			ent.set_h(ruby_h);
		break;
	}
}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const collectible& _collectible
) {

	_stream<<"collectible["<<_collectible.ent.get_origin()<<"]";
	return _stream;
}
