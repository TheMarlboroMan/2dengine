#include "app/boss.h"
#include "app/definitions.h"

using namespace app;

boss::boss(
	d2d::collision::point _origin,
	int _skill
):
	ent(_origin, w, h)
{

	switch(_skill) {

		default:
		case skill_easy: skill=skills::easy; break;
		case skill_normal: skill=skills::normal; break;
		case skill_hard: skill=skills::hard; break;
	}
}

void boss::tic(
	ldtools::tdelta _delta
) {

}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const boss& _monster
) {

	_stream<<"boss["<<_monster.ent.get_origin()<<"]";
	return _stream;
}
