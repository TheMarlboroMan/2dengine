#include "dfwimpl/config.h"
#include "dfwimpl/state_driver.h"
#include "app/env.h"
#include "controller/controller_states.h"

/*#include <lm/file_logger.h>*/
#include <lm/ostream_logger.h>
#include <lm/log.h>

#include <dfw/kernel.h>

#include <tools/arg_manager.h>

#include <ldt/sdl_tools.h>
#include <ldt/log.h>

int main(int argc, char ** argv)
{
	//TODO: It would be GREAT if this shit could be just packed into the lib as "main".
	//
	

	app::env env;


	//Init libdansdl2 log.
	ldt::log_lsdl::set_type(ldt::log_lsdl::types::out);
	//ldt::log_lsdl::set_type(ldt::log_lsdl::types::null);
	//ldt::log_lsdl::set_filename("logs/libdansdl2.log");

	//Argument controller.
	tools::arg_manager carg(argc, argv);

	//Init application log.
	//std::string log_path{env.get_app_path()+"logs/app.log"};
	//lm::file_logger log_app(log_path.c_str());
	lm::ostream_logger log_app(std::cout);
	lm::log(log_app).info()<<"starting main process..."<<std::endl;

	//Init...
	try {
		lm::log(log_app).info()<<"init sdl2..."<<std::endl;
		if(!ldt::sdl_init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK)) {
			throw std::runtime_error("unable to init sdl2");
		}

		lm::log(log_app).info()<<"creating kernel..."<<std::endl;
		dfw::kernel kernel(log_app, carg);

		lm::log(log_app).info()<<"init app config..."<<std::endl;
		dfwimpl::config config(env);

		lm::log(log_app).info()<<"create state driver..."<<std::endl;
		int initial_state=controller::state_main;
		dfwimpl::state_driver sd(kernel, config, env, initial_state);

		lm::log(log_app).info()<<"init state driver..."<<std::endl;
		sd.init(kernel);

		lm::log(log_app).info()<<"finish main proccess"<<std::endl;
	}
	catch(std::exception& e) {
		std::cerr<<"Interrupting due to exception: "<<e.what()<<std::endl;
		lm::log(log_app).error()<<"an error happened "<<e.what()<<std::endl;
		lm::log(log_app).info()<<"stopping sdl2..."<<std::endl;
		ldt::sdl_shutdown();

		return 1;
	}

	lm::log(log_app).info()<<"stopping sdl2..."<<std::endl;
	ldt::sdl_shutdown();
	return 0;
}

