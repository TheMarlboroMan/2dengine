#include "app/env.h"
#include "controller/controller_states.h"
#include "dfwimpl/config.h"
#include "dfwimpl/state_driver.h"
#include <lm/ostream_logger.h>
#include <ldt/sdl_tools.h>

int main(int argc, char ** argv)
{
	app::env env;
		
	//Init application log.
	//std::string log_path{env.get_app_path()+"logs/app.log"};
	//lm::fileapp_log app_log(log_path.c_str());
	lm::ostream_logger app_log(std::cout);

	try {

		//Setup the application configuration
		lm::log(app_log).info()<<"setting up config..."<<std::endl;
		const std::string config_file{env.get_usr_path()+"/resources/runtime/config.json"};
		dfwimpl::config config(config_file);

		//Init the state driver, this should NOT start system stuff yet!.
		lm::log(app_log).info()<<"building state driver..."<<std::endl;
		int initial_state=controller::state_main;
		dfwimpl::state_driver sd(config, app_log, env, initial_state);

		//init sdl2 wrapper log.
		ldt::log_lsdl::set_type(ldt::log_lsdl::types::out);
		//ldt::log_lsdl::set_type(ldt::log_lsdl::types::null);
		//ldt::log_lsdl::set_filename("logs/libdansdl2.log");

		lm::log(app_log).info()<<"init sdl2..."<<std::endl;
		if(!ldt::sdl_init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK)) {

			throw std::runtime_error("unable to init sdl2");
		}

		//Start the kernel. 
		tools::arg_manager carg(argc, argv);
		lm::log(app_log).info()<<"creating kernel..."<<std::endl;
		dfw::kernel kernel(app_log, carg);

		//Init system-dependant stuff and application stuff.
		lm::log(app_log).info()<<"init state driver..."<<std::endl;
		sd.init(kernel);

		//Finally enter the application space.
		lm::log(app_log).info()<<"start state driver..."<<std::endl;
		sd.start(kernel);

		lm::log(app_log).info()<<"finished main process, stopping sdl2..."<<std::endl;
		ldt::sdl_shutdown();

		return 0;
	}
	catch(std::exception& e) {

		std::cerr<<"Interrupting due to exception: "<<e.what()<<std::endl;
		lm::log(app_log).error()<<"an error happened "<<e.what()<<std::endl;
		lm::log(app_log).info()<<"stopping sdl2..."<<std::endl;
		ldt::sdl_shutdown();

		return 1;
	}
}
