#pragma once

#include "config.h"
#include "app/service_provider.h"

#include <lm/logger.h>
#include <dfw/state_driver_interface.h>
#include <dfw/controller_interface.h>
#include <ldtools/time_definitions.h>

#ifdef IS_DEBUG_BUILD

#include <dfw/input_converter_identity.h>
#include <dfw/input_recorder_interface.h>
#include <dfw/input_generator_interface.h>

#endif

#include <memory>

namespace dfwimpl {

class state_driver:
	public dfw::state_driver_interface {

	public:
	                                state_driver(dfwimpl::config& config, lm::logger&, const appenv::env&, int);


	virtual void                    common_pre_loop_input(dfw::input& input, ldtools::tdelta delta);
	virtual void                    common_pre_loop_step(ldtools::tdelta delta);
	virtual void                    common_loop_input(dfw::input& input, ldtools::tdelta delta);
	virtual void                    common_loop_step(ldtools::tdelta delta);
	virtual void                    prepare_state(int, int);
	virtual ldtools::tdelta         get_max_timestep() const {return 0.03;}
	virtual	void					init(dfw::kernel&);

	private:

	bool                            validate_state(int);
	void                            prepare_video(dfw::kernel&, dfw::window_info, bool);
	void                            prepare_audio(dfw::kernel&);
	void                            prepare_input(dfw::kernel&);
	void                            prepare_resources(dfw::kernel&);
	void                            virtualize_input(dfw::input& input, int);

	dfw::window_info                get_video_init_data() const;
	dfw::audio_info                 get_audio_init_data() const;
	void                            load_resources(dfw::kernel&);
	void                            load_fonts(ldtools::ttf_manager&);
	void                            register_controllers(dfw::kernel&);
	void                            ready_resources(dfw::kernel&);
	std::vector<dfw::input_pair>    get_input_pairs() const;
	void                            start_app(const tools::arg_manager&, dfw::input&);
	int                             get_input_axis_threshold() const;
	std::string                     build_resource_path(const std::string, const std::string) const;

	//references
	dfwimpl::config&                config;
	lm::logger&                     log;
	const appenv::env&              env;

	//data.
	std::unique_ptr<app::service_provider>  service_provider{nullptr};

#ifdef IS_DEBUG_BUILD

	void                            setup_input_recorder(dfw::input&, const std::string&);
	dfw::input_converter_identity                     input_converter;
	std::unique_ptr<dfw::input_recorder_interface>    input_recorder{nullptr};
	std::unique_ptr<dfw::input_generator_interface>   input_generator{nullptr};
#endif
	

	typedef std::unique_ptr<dfw::controller_interface>	ptr_controller;
	ptr_controller                  c_main;
	ptr_controller                  c_menu;
	ptr_controller                  c_pause;
	ptr_controller                  c_controls;
	ptr_controller                  c_options;
	ptr_controller                  c_show_text;
	//[new-controller-property-mark]
};

}
