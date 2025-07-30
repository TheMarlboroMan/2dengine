#include "app/boss.h"
#include "app/definitions.h"
#include "d2d/motion/mover.h"

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


	switch(stage) {

		case stages::setup:
			stage_setup();
			[[fallthrough]];
		case stages::appear:

			return stage_appear(_delta);

		case stages::stage_1:
			return stage_one(_delta);
	}
}

void boss::stage_setup() {

	appear_y_target=ent.get_y();

	//Set the boss outside the screen.
	ent.set_y(app::logic_screen_h+(2*app::tile_h));

	stage=stages::appear;
}

void boss::stage_appear(
	ldtools::tdelta _delta
) {

	//Descend until the y_target is reached.
	d2d::motion::mover mv;
	mv.apply_y(ent, appear_y_speed, _delta);

	if(ent.get_y() <= appear_y_target) {

		//Ready the entity position and movement...
		ent.set_y(appear_y_target);
		ent.set_motion_vector({horizontal_speed, 0.});

		//TODO: Maybe pause????
		stage=stages::stage_1;

		return;
	}
}

void boss::stage_one(
	ldtools::tdelta _delta
) {

	//Move left and right... and little more.

	d2d::motion::mover mv;
	mv.apply(ent, _delta);

	const auto x=ent.get_x();
	if(x >= phase_one_x_right_limit) {

		ent.set_motion_vector({-horizontal_speed, 0.});
	}

	if(x <= phase_one_x_left_limit) {

		ent.set_motion_vector({horizontal_speed, 0.});
	}

	//TODO: What now??
}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const boss& _monster
) {

	_stream<<"boss["<<_monster.ent.get_origin()<<"]";
	return _stream;
}
