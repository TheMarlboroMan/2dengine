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
	timeouts.add(timeout_summon_skull, 0., 0., true);

	appear_x_start=ent.get_x();
	
	//Save the target y for the first phase...
	appear_y_target=ent.get_y();
	reset();

	stage=stages::setup_stage_7;
}

int boss::get_volley_count() const {

	switch(skill) {

		case skills::easy: return 3;
		case skills::normal: return 4;
		case skills::hard: return 5;
	}
}

void boss::setup_facing(
	double _target_x
) {

	const auto center=ldt::get_center(ent.get_box()).x;

	facing=_target_x <= center
		? faces::left
		: faces::right;
}

void boss::notify_skull_destroyed(
	int _skulls_left
) {

	if(0!=_skulls_left) {

		return;
	}

	if(stage==stages::stage_1) {

		ready_pause(stages::setup_stage_2, 1.5);
		//TODO: Sky might flash?
	}
	else if(stage==stages::stage_5) {

		stage_five_hit_skull();
		//TODO: Sky might flash?
	}
	else if(stage==stages::stage_9) {

		//TODO: defeated on easy!!
		//TODO: Sky might flash?
	}
}

void boss::reset() {

	//Set the boss outside the screen, at its x starting point.
	ent.set_x(appear_x_start);
	ent.set_y(app::logic_screen_h+(2*app::tile_h));
	ent.commit_box();

	volley_count=0;
	volley_total=0;
	volley_angle=0;
	uses_left_hand=true;
	distance_total=0.;
	distance_moved=0.;
	skull_count=0;

	timeouts.reset().pause();

	stage=stages::setup_appear;
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
		case stages::setup_appear:
			setup_stage_appear();
			[[fallthrough]];
		case stages::appear:
			return stage_appear(_delta);
		case stages::setup_stage_1:
			setup_stage_one();
			[[fallthrough]];
		case stages::stage_1:
			return stage_one(_delta);
		case stages::setup_stage_2:
			setup_stage_two();
			[[fallthrough]];
		case stages::stage_2:
			return stage_two(_delta);
		case stages::setup_stage_3:
			setup_stage_three();
			[[fallthrough]];
		case stages::stage_3:
			return stage_three(_delta);
		case stages::setup_stage_4:
			setup_stage_four();
			[[fallthrough]];
		case stages::stage_4:
			return stage_four();
		case stages::setup_stage_5:
			setup_stage_five();
			[[fallthrough]];
		case stages::stage_5:
			return stage_five(_delta);
		case stages::setup_stage_6:
			setup_stage_six();
			[[fallthrough]];
		case stages::stage_6:
			return stage_six(_delta);
		case stages::setup_stage_7:
			setup_stage_seven();
			[[fallthrough]];
		case stages::stage_7:
			return stage_seven(_delta);
		case stages::setup_stage_8:
			setup_stage_eight();
			[[fallthrough]];
		case stages::stage_8:
			return stage_eight(_delta);
		case stages::setup_stage_9:
			setup_stage_nine();
			[[fallthrough]];
		case stages::stage_9:
			return stage_nine(_delta);
	}
}

void boss::do_side_movement(
	ldtools::tdelta _delta,
	double _velocity
) {

	d2d::motion::mover mv;
	mv.apply_and_commit(ent, _delta);

	const auto x=ent.get_x();
	if(x >= x_right_limit) {

		ent.set_motion_vector({-_velocity, 0.});
	}

	if(x <= x_left_limit) {

		ent.set_motion_vector({_velocity, 0.});
	}
}

void boss::ready_targeted_movement(
	d2d::collision::point _target,
	double _velocity
) {

	d2d::motion::motion_vector vec=ldt::vector_from_points(
		//TODO: Is this the center??
		ent.get_origin(),
		_target
	);

	distance_moved=0;
	distance_total=vec.magnitude();

	ent.set_motion_vector(vec.normalize() * _velocity);
}

bool boss::do_targeted_movement(
	ldtools::tdelta _delta
) {

	d2d::motion::mover mv;
	mv.apply_and_commit(ent, _delta);
	distance_moved+=ent.get_motion_vector().magnitude() * _delta;

	return distance_moved >= distance_total;
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

void boss::setup_stage_appear() {

	double velocity=first_appears 
			? first_appear_y_speed
			: subsequent_appear_y_speed;

	d2d::collision::point target(
		appear_x_start,
		appear_y_target
	);

	ready_targeted_movement(target, velocity);
	stage=stages::appear;
}

void boss::stage_appear(
	ldtools::tdelta _delta
) {

	if(do_targeted_movement(_delta)) {

		//Ready the entity position... 
		first_appears=false;
		ent.set_y(appear_y_target);
		ready_pause(stages::setup_stage_1, 2.);
		return;
	}
}

void boss::setup_stage_one() {

	//Spawn the skulls!!
	bmi->boss_spawn_skull(1, 2.);
	bmi->boss_spawn_skull(2, 2.);
	volley_count=0;
	ent.set_motion_vector({phase_one_horizontal_speed, 0.});

	timeouts.target(timeout_fire, phase_one_fire_delay)
		.restart();

	stage=stages::stage_1;
}

void boss::stage_one(
	ldtools::tdelta _delta
) {

	//Move left and right... and shoot alternately from each "hand".
	do_side_movement(_delta, phase_one_horizontal_speed);

	if(timeouts.is_finished(timeout_fire)) {

		//Shoot alternatively from each hand!!!
		auto origin=ent.get_origin();
		origin.x+=volley_count++ % 2 
			? left_hand_offset
			: right_hand_offset;

		bmi->boss_create_targeted_projectile(origin, 100.);
		timeouts.restart(timeout_fire);
	}
}

void boss::setup_stage_two() {

	volley_count=0;
	volley_total=get_volley_count();
	timeouts.target(timeout_fire, phase_two_fire_delay)
		.restart();

	stage=stages::stage_2;
}

void boss::stage_two(
	ldtools::tdelta
) {

	//Shoot triple volleys against the player...
	if(timeouts.is_finished(timeout_fire)) {

		//TODO: Maybe the angle closes as we are nearer?
		bmi->boss_create_targeted_projectile(ent.get_origin(), 120., 0);
		bmi->boss_create_targeted_projectile(ent.get_origin(), 120., 30);
		bmi->boss_create_targeted_projectile(ent.get_origin(), 120., -30);
		timeouts.restart(timeout_fire);

		if(++volley_count >= volley_total) {

			stage=stages::setup_stage_3;
			return;
		}
	}
}

void boss::setup_stage_three() {

	//Set the position to the center of the screen.
	d2d::collision::point target{
		(double)((app::logic_screen_w / 2) - w),
		(double)(app::logic_screen_h / 2)
	};

	ready_targeted_movement(target, phase_three_speed);
	stage=stages::stage_3;
}

void boss::stage_three(
	ldtools::tdelta _delta
) {

	//We are not accounting for any extra distance but this is a good enough
	//approximation... Once the boss reaches the target destination we 
	//start a new pause, then its onwards to stage 4.
	if(do_targeted_movement(_delta)) {

		ready_pause(stages::setup_stage_4, 2.);
	}
}

void boss::setup_stage_four() {

	volley_count=0;
	volley_total=10*get_volley_count();
	timeouts.target(timeout_fire, phase_four_fire_delay)
		.restart();
	stage=stages::stage_4;
}

void boss::stage_four() {

	if(!timeouts.is_finished(timeout_fire)) {

		return;
	}

	if(++volley_count >= volley_total) {

		ready_pause(stages::setup_stage_5, 4.);
		return;
	}

	const auto origin=ldt::get_center(ent.get_box());
	bmi->boss_create_directed_projectile(origin, volley_angle, 100.);
	bmi->boss_create_directed_projectile(origin, volley_angle+120, 100.);
	bmi->boss_create_directed_projectile(origin, volley_angle+240, 100.);

	volley_angle+=6;

	timeouts.restart(timeout_fire);
}

void boss::setup_stage_five() {

	skull_count=0;
	volley_count=0;
	volley_total=get_volley_count();
	uses_left_hand=true;

	timeouts.target(timeout_summon_skull, phase_five_summon_skull_delay)
		.restart();

	timeouts.target(timeout_fire, phase_five_volley_delay)
		.restart();

	ent.set_motion_vector({phase_five_horizontal_speed, 0.});

	stage=stages::stage_5;
}

void boss::stage_five(
	ldtools::tdelta _delta
) {

	//A bit of faster movement, but only when not firing!
	if(0==volley_count) {

		do_side_movement(_delta, phase_five_horizontal_speed);
	}

	//Skulls are summoned in sequence as they are destroyed.
	if(timeouts.is_finished(timeout_summon_skull)) {

			int spawn_id=0;
			switch(skull_count) {
				case 0: spawn_id=7; break;
				case 1: spawn_id=3; break;
				case 2: spawn_id=4; break;
				case 3: spawn_id=8; break;
			}

			bmi->boss_spawn_skull(spawn_id, 2.);
			timeouts.reset(timeout_summon_skull);
	}

	//shots go in volleys of three so we can't camp behind the skulls now!
	if(timeouts.is_finished(timeout_fire)) {

		auto origin=ent.get_origin();
		origin.x+=uses_left_hand
			? left_hand_offset
			: right_hand_offset;

		//When starting the volley, lock the target and the short delay.
		if(0==volley_count) {

			timeouts.target(timeout_fire, phase_five_fire_delay)
				.restart();

			const auto target=bmi->boss_get_target();
			volley_angle=ldt::angle_between(origin, target);
		}

		//TODO: All these 100. should be constants somewhere.
		bmi->boss_create_directed_projectile(origin, volley_angle, 100.);

		//Volley done? Change the delay and hand.
		if(++volley_count >= volley_total) {

			timeouts.target(timeout_fire, phase_five_volley_delay)
				.restart();
			volley_count=0;
			uses_left_hand=!uses_left_hand;
			return;
		}

		timeouts.restart(timeout_fire);
	}
}

void boss::stage_five_hit_skull() {

	//When a skull is hit in phase five we must check if more skulls will
	//spawn after a timeout or if the whole phase is done.
	++skull_count;

	if(skull_count==4) {

		ready_pause(stages::setup_stage_6, 2.);
		return;
	}

	timeouts.restart(timeout_summon_skull);
}

void boss::setup_stage_six() {

	volley_count=0;
	volley_total=get_volley_count();
	timeouts.target(timeout_fire, phase_six_fire_delay)
		.restart();

	stage=stages::stage_6;
}

void boss::stage_six(
	ldtools::tdelta
) {

	//Shoot volleys against the player, each shot has different velocity!
	if(timeouts.is_finished(timeout_fire)) {

		bmi->boss_create_targeted_projectile(ent.get_origin(), 140., 0);
		bmi->boss_create_targeted_projectile(ent.get_origin(), 100., 20);
		bmi->boss_create_targeted_projectile(ent.get_origin(), 100., -20);
		timeouts.restart(timeout_fire);

		if(++volley_count >= volley_total) {

			stage=stages::setup_stage_7;
			return;
		}
	}
}

void boss::setup_stage_seven() {

	//Set the position to the right, at the bottom...
	d2d::collision::point target{
		(double)((app::logic_screen_w) - (2*w)),
		(double)(h)
	};

	ready_targeted_movement(target, phase_seven_speed);
	stage=stages::stage_7;
}

void boss::stage_seven(
	ldtools::tdelta _delta
) {

	if(do_targeted_movement(_delta)) {

		ready_pause(stages::setup_stage_8, 2.);
	}
}

void boss::setup_stage_eight() {

	volley_count=0;
	volley_total=5;

	timeouts.target(timeout_fire, phase_eight_fire_delay)
		.restart();

	//Summon the particles... We need the gap function and shit??
	int gap=get_volley_gap(volley_count);
	for(int i=2; i<15; i++) {

		if(i==gap)  {

			continue;
		}

		double x=ent.get_origin().x; 
		double y=(i*tile_h)-(tile_h/2);
		bmi->boss_spawn_particle({x, y}, app::prt_flame);
	}

	stage=stages::stage_8;
}

void boss::stage_eight(
	ldtools::tdelta 
) {

	//Shot a "wall" of projectiles with a gap.
	if(timeouts.is_finished(timeout_fire)) {

		int gap=get_volley_gap(volley_count);

		for(int i=2; i<15; i++) {
		
			if(gap==i) {

				continue;
			}

			double x=ent.get_origin().x; 
			double y=(i*tile_h)-(tile_h/2);
			d2d::collision::point origin{x, y};
			bmi->boss_create_linear_projectile(origin, -75.);

			for(int j=0; j<4; j++) {
				bmi->boss_spawn_particle(origin, app::prt_smoke);
			}
		}

		timeouts.restart(timeout_fire);
		if(++volley_count >= volley_total) {

			ready_pause(stages::setup_stage_9, 6.);
			return;
		}

		//Summon the particles again for the next volley!
		gap=get_volley_gap(volley_count);
		for(int i=2; i<15; i++) {
	
			if(i==gap) {

				continue;
			}
			double x=ent.get_origin().x; 
			double y=(i*tile_h)-(tile_h/2);
			bmi->boss_spawn_particle({x, y}, app::prt_flame);
		}
	}
}

void boss::setup_stage_nine() {

	skull_count=0;
	volley_count=0;
	//TODO:
	volley_total=0;
	uses_left_hand=true;

	timeouts.target(timeout_summon_skull, phase_nine_summon_skull_delay)
		.restart();

	timeouts.target(timeout_fire, phase_nine_volley_delay)
		.restart();

	ent.set_motion_vector({phase_nine_horizontal_speed, 0.});

	stage=stages::stage_9;
}

void boss::stage_nine(
	ldtools::tdelta _delta
) {

	//Again, no firing while moving!!
	if(0==volley_count) {

		do_side_movement(_delta, phase_nine_horizontal_speed);
	}

	//TODO: I thought, since the boss is on the low part maybe the player
	//has to move on the upper side while the boss shots upwards... But of
	//course, what about the skulls??

	//How about WIDE shots?
}

int boss::get_volley_gap(
	int _count
) const {

	switch(_count) {

		case 0: return 4;
		case 1: return 9;
		case 2: return 12;
		case 3: return 9;
		case 4: return 2;
	}

	return 0;
}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const boss& _monster
) {

	_stream<<"boss["<<_monster.ent.get_origin()<<"]";
	return _stream;
}
