#pragma once

#ifdef IS_DEBUG_BUILD

#include "app/service_provider.h"
#include "app/env.h"
#include "app/entity.h"

#include <dfw/controller_interface.h>

#include <d2d/motion/definitions.h>
#include <d2d/collision/definitions.h>
#include <d2d/collision/ray_aabb_finder.h>

#include <vector>
#include <lm/logger.h>
#include <ldv/camera.h>
#include <ldt/segment_2d.h>

namespace controller {

class test:
	public dfw::controller_interface {

	public:

	                            test(app::service_provider&);

	virtual void                loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void                draw(ldv::screen&, int);
	virtual void                awake(dfw::input& /*input*/);
	virtual void                slumber(dfw::input& /*input*/);
	virtual bool                can_leave_state() const {return true;}

	private:

	void                        real_loop(dfw::input&, const dfw::loop_iteration_data&);
	void                        test_loop(dfw::input&, const dfw::loop_iteration_data&);
	void                        real_draw(ldv::screen&, int);
	void                        test_draw(ldv::screen&, int);

	ldt::segment_2d<double>     get_player_ray() const;

	app::service_provider&      sp;
	const appenv::env&          env;
	lm::logger&                 logger;

	ldv::camera                 camera;
	app::entity                 player;
	d2d::motion::motion_vector  player_vector;
	ldt::segment_2d<double>     player_ray;
	bool                        player_on_air{false};

	bool                        test_mode{false};
	bool                        next_tic{true};

	std::vector<app::entity>    obstacles;
	std::vector<d2d::collision::ray_aabb_info> collision_responses;

};

}

#endif
