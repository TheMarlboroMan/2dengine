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

	//The pause state timer starts... paused.
	timeouts.add(timeout_pause, 0., 0., true);
	timeouts.add(timeout_fire, 0., 0., true);
}

void boss::set_boss_map_interface(
	boss_map_interface& _bmi
) {

	bmi=&_bmi;
}

void boss::tic(
	ldtools::tdelta _delta
) {

	timeouts.tic(_delta);

	switch(stage) {

		case stages::pause:
			return stage_pause(_delta);

		case stages::setup:
			stage_setup();
			[[fallthrough]];
		case stages::appear:

			return stage_appear(_delta);

		case stages::setup_stage_1:
			setup_stage_one();
			[[fallthrough]];
		case stages::stage_1:
			return stage_one(_delta);
	}
}

void boss::ready_pause(
	boss::stages _stage,
	ldtools::tdelta _timer
) {

	after_pause_stage=_stage;
	stage=stages::pause;
	timeouts.target(timeout_pause, _timer)
		.restart();
}

void boss::stage_pause(
	ldtools::tdelta
) {

	//This is every pause that the boss makes...
	if(timeouts.is_finished(timeout_pause)) {

		stage=after_pause_stage;
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

		ready_pause(stages::setup_stage_1, 2.);

		return;
	}
}

void boss::setup_stage_one() {

	//Spawn the skulls!!
	bmi->boss_spawn_skull(1);
	bmi->boss_spawn_skull(2);
	stage=stages::stage_1;

	timeouts.target(timeout_fire, phase_one_fire_delay)
		.restart();
}

void boss::stage_one(
	ldtools::tdelta _delta
) {

	//Move left and right... and shoot alternately from each "hand".
	d2d::motion::mover mv;
	mv.apply(ent, _delta);

	const auto x=ent.get_x();
	if(x >= phase_one_x_right_limit) {

		ent.set_motion_vector({-horizontal_speed, 0.});
	}

	if(x <= phase_one_x_left_limit) {

		ent.set_motion_vector({horizontal_speed, 0.});
	}

	if(timeouts.is_finished(timeout_fire)) {

		//TODO: Shoot alternatively from each hand!!!
		bmi->boss_create_targeted_projectile(ent.get_origin());
		timeouts.restart(timeout_fire);
	}
}

void boss::notify_skull_destroyed(
	int _skulls_left
) {

	//TODO: 
	std::cout<<"SKULL HIT, LEFT: "<<_skulls_left<<std::endl;

}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const boss& _monster
) {

	_stream<<"boss["<<_monster.ent.get_origin()<<"]";
	return _stream;
}
