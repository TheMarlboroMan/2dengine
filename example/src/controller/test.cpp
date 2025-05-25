#ifdef IS_DEBUG_BUILD

#include "controller/test.h"

#include "app/input.h"
#include "app/definitions.h"

#include <d2d/motion/mover.h>
#include <d2d/collision/ray_aabb_finder.h>
#include <d2d/collision/ray_aabb_finder.h>
#include <d2d/collision/ray_aabb_solver.h>
#include <d2d/collision/ray_builder.h>
#include <d2d/collision/aabb_checker.h>
#include <d2d/collision/tools.h>
#include <ldv/draw.h>
#include <ldt/segment_2d.h>


#include <iostream>

using namespace controller;

test::test(
	app::service_provider& _sp
):
	sp{_sp},
	env{_sp.get_env()},
	logger{_sp.get_logger()},
	camera{ {0,0,app::logic_screen_w, app::logic_screen_h}, {0,0}, ldv::camera::tsystem::cartesian},
	player{17, 16, 10, 25}
{

	auto create_block=[&](int _grid_x, int _grid_y) -> void {

		obstacles.push_back({_grid_x*16, _grid_y*16, 16, 16});
	};

	create_block(0,0);
	create_block(0,1);
	create_block(0,2);
	create_block(0,3);
	create_block(0,4);
	create_block(0,5);
	create_block(0,6);
	create_block(0,7);
	create_block(0,8);
	create_block(0,9);
	create_block(0,10);
	create_block(1,0);
	create_block(2,0);
	create_block(3,0);
	create_block(4,0);
	create_block(5,0);
	create_block(5,5);
	create_block(6,0);
	create_block(8,0);
	create_block(8,1);
	create_block(8,2);
	create_block(8,3);
	create_block(8,4);
	create_block(9,0);
	create_block(10,0);
	create_block(11,0);
	create_block(11,1);
	create_block(11,2);
	create_block(11,0);
	create_block(12,0);
	create_block(13,0);
	create_block(2,9);
	create_block(3,9);
	create_block(4,9);
	create_block(3,8);
	create_block(3,7);
	create_block(3,6);
	create_block(3,10);
	create_block(3,11);
	create_block(10,10);

	obstacles.push_back({180, 180, 4, 4});
	obstacles.push_back({180, 200, 4, 4});
	obstacles.push_back({100, 150, 100, 4});
	obstacles.push_back({140, 80, 4, 100});
}

void test::awake(
	dfw::input& /*input*/
) {

}

void test::slumber(
	dfw::input& /*input*/
) {

}


void test::loop(
	dfw::input& _input,
	const dfw::loop_iteration_data& _lid
) {

	if(_input.is_input_down(app::input::escape)
		|| _input().is_key_down(SDL_SCANCODE_ESCAPE)
		|| _input().is_exit_signal()
	) {

		set_leave(true);
		return;
	}


	if(_input.is_input_pressed(app::input::pause)) {

		test_mode=!test_mode;
	}

	test_mode 
		? test_loop(_input, _lid)
		: real_loop(_input, _lid);
}

void test::real_loop(
	dfw::input& _input,
	const dfw::loop_iteration_data& _lid
) {

	if(_input.is_input_down(app::input::tic)) {

		next_tic=true;
	}

	if(!next_tic) {

		return;
	}

	int input_x=0;
	bool tries_to_jump=false;

	if(_input.is_input_pressed(app::input::left)) {

		input_x=-1;
	}
	else if(_input.is_input_pressed(app::input::right)) {

		input_x=1;
	}

	tries_to_jump=false;
	if(_input.is_input_pressed(app::input::jump)) {

		tries_to_jump=true;
	}

	double speed=100.;
	player_vector.x=input_x*speed;

	//Check if player is on air by doing a quick check against a modified box...
	auto modified_box=player.get_box();
	modified_box.origin.y-=1.0;
	d2d::collision::aabb_checker on_air_checker;
	player_on_air=!on_air_checker.has_collision(modified_box, obstacles);

	if(tries_to_jump && !player_on_air) {

		player_vector.y=300.;
		player_on_air=true;
	}


//	if(player_on_air) {

//		player_vector.y-=600. * _lid.delta;
//	}
	player_vector.y-=600. * _lid.delta;

	//Ready some stuff for drawing purposes!
	d2d::motion::mover mover;
	mover.apply(player, player_vector, _lid.delta);

	//Very important: we only need the current fraction of the ray!!!
	d2d::collision::ray_builder rb;
	auto ray=rb.get_previous(player, player_vector)*_lid.delta;

//TODO: Not working when we jump and PUSH AGAINST A WALL...
//a collision is detected there... what do we do???
//
std::cout<<player.get_box()<<std::endl;

	d2d::collision::ray_aabb_finder finder;
	collision_responses=finder.find(ray, player, obstacles);
	if(collision_responses.size()) {

		if(1 < collision_responses.size()) {

//			next_tic=false;
		}

		d2d::collision::ray_aabb_solver solver;
		int edges=solver.solve(player, collision_responses);

		if(edges & d2d::collision::ray_aabb_solver::bottom) {

			player_vector.y=0.;
		}

		if(edges & d2d::collision::ray_aabb_solver::top) {

			player_vector.y=0;
		}
	}

	player.commit_box();

}


void test::draw(
	ldv::screen& _screen,
	int
) {

	ldv::draw dr(_screen, &camera);
	dr.clear(dr.black);
	test_mode 
		? test_draw(_screen, 0)
		: real_draw(_screen, 0);
	dr.update();
}

void test::real_draw(
	ldv::screen& _screen,
	int
) {

	ldv::draw dr(_screen, &camera);

	dr.box(player.get_box(), dr.blue);

	std::vector<ldv::rgba_color> colors={dr.gray, dr.white};
	int i=0;

	for(const auto& obstacle : obstacles) {

		dr.box(obstacle.get_box(), colors[i++%2]);
	}

}

void test::test_draw(
	ldv::screen& _screen,
	int
) {

	ldv::draw dr(_screen, &camera);

	dr.box(player.get_box(), dr.blue);

	std::vector<ldv::rgba_color> colors={dr.gray, dr.white};
	int i=0;

	for(const auto& obstacle : obstacles) {

		dr.box(obstacle.get_box(), colors[i++%2]);
	}

	dr.ray(player_ray, dr.red);

	for(auto& response : collision_responses) {

		dr.box(response.obstacle->get_box(), dr.red);

		//Drawing the normal would be fun xD we would need a side of
		//the original and shit and we just don't want it, so here, just
		//use the point as the center.
		auto normal=response.normal * 10.;
		dr.ray({player_ray.point, normal}, dr.white);
	}
}

void test::test_loop(
	dfw::input& _input,
	const dfw::loop_iteration_data& _lid
) {

	if(_input.is_input_pressed(app::input::jump)) {

		collision_responses.clear();
	}

	double input_x=0, input_y=0;

	if(_input.is_input_pressed(app::input::left)) {

		input_x=-1;
	}
	else if(_input.is_input_pressed(app::input::right)) {

		input_x=1;
	}

	if(_input.is_input_pressed(app::input::up)) {

		input_y=1;
	}
	else if(_input.is_input_pressed(app::input::down)) {

		input_y=-1;
	}

	double speed=100.;
	player_vector.x=input_x*speed;
	player_vector.y=input_y*speed;

	//Ready some stuff for drawing purposes!
	d2d::collision::ray_builder rb;
	player_ray=rb.get(player, player_vector);

std::cout<<player.get_box()<<std::endl;

//	if(0==collision_responses.size()) {

		d2d::motion::mover mover;
		mover.apply(player, player_vector, _lid.delta);

		//Very important: we only need the current fraction of the ray!!!
		auto ray=rb.get_previous(player, player_vector)*_lid.delta;

		d2d::collision::ray_aabb_finder finder;
		collision_responses=finder.find(ray, player, obstacles);
		if(collision_responses.size()) {

			d2d::collision::ray_aabb_solver solver;
			solver.solve(player, collision_responses);
		}
//	}

	player.commit_box();
}


#endif
