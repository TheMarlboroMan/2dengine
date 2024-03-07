#include "app/collectible.h"
#include "app/definitions.h"

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
		case yellow_key:
		case blue_key:
		case red_key:
			ent.set_w(key_w);
			ent.set_h(key_h);
		break;
	}

	int x=ent.get_x();
	x+=(app::tile_w - ent.get_w()) / 2;
	ent.set_x(x);
}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const collectible& _collectible
) {

	_stream<<"collectible["<<_collectible.ent.get_origin()<<"]";
	return _stream;
}
