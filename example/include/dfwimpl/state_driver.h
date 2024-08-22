#pragma once

#include "config.h"
#include "app/service_provider.h"

#include <lm/logger.h>
#include <dfw/state_driver_interface.h>
#include <dfw/controller_interface.h>

#include <memory>

namespace dfwimpl {

class state_driver:
	public dfw::state_driver_interface {

	public:
	                                state_driver(dfwimpl::config& config, lm::logger&, const appenv::env&, int);


	virtual void                    common_pre_loop_input(dfw::input& input, float delta);
	virtual void                    common_pre_loop_step(float delta);
	virtual void                    common_loop_input(dfw::input& input, float delta);
	virtual void                    common_loop_step(float delta);
	virtual void                    prepare_state(int, int);
	virtual float                   get_max_timestep() const {return 0.03f;}
	virtual	void					init(dfw::kernel&);

	private:

	void                            prepare_video(dfw::kernel&);
	void                            prepare_audio(dfw::kernel&);
	void                            prepare_input(dfw::kernel&);
	void                            prepare_resources(dfw::kernel&);
	void                            register_controllers(dfw::kernel&);
	void                            virtualize_input(dfw::input& input);
	void                            load_resources();
	void                            start_app(const tools::arg_manager&);

	//references
	dfwimpl::config&                config;
	lm::logger&                     log;
	const appenv::env&              env;
	std::unique_ptr<app::service_provider>  service_provider{nullptr};
	

	typedef std::unique_ptr<dfw::controller_interface>	ptr_controller;
	ptr_controller                  c_main;
	ptr_controller                  c_menu;
	ptr_controller                  c_pause;
	ptr_controller                  c_controls;
	//[new-controller-property-mark]
};

}
