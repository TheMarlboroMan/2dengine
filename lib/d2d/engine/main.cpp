#include "d2d/engine/main.h"
#include <ldt/sdl_tools.h>

using namespace d2d::engine;

int d2d::engine::main(
	int _argc, 
	char ** _argv,
	dfw::state_driver_interface& _sd,
	lm::logger& _logger
) {

	//TODO: This should be customizable...
	//Init libdansdl2 log.
	ldt::log_lsdl::set_type(ldt::log_lsdl::types::out);
	//ldt::log_lsdl::set_type(ldt::log_lsdl::types::null);
	//ldt::log_lsdl::set_filename("logs/libdansdl2.log");

	//Argument controller.
	tools::arg_manager carg(_argc, _argv);

	//Init...
	try {
		lm::log(_logger).info()<<"init sdl2..."<<std::endl;
		if(!ldt::sdl_init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK)) {
			throw std::runtime_error("unable to init sdl2");
		}

		lm::log(_logger).info()<<"creating kernel..."<<std::endl;
		dfw::kernel kernel(_logger, carg);

		lm::log(_logger).info()<<"init state driver..."<<std::endl;
		_sd.init(kernel);

		lm::log(_logger).info()<<"start state driver..."<<std::endl;
		_sd.start(kernel);

		lm::log(_logger).info()<<"finish main proccess"<<std::endl;
	}
	catch(std::exception& e) {

		std::cerr<<"Interrupting due to exception: "<<e.what()<<std::endl;
		lm::log(_logger).error()<<"an error happened "<<e.what()<<std::endl;
		lm::log(_logger).info()<<"stopping sdl2..."<<std::endl;
		ldt::sdl_shutdown();

		return 1;
	}

	lm::log(_logger).info()<<"stopping sdl2..."<<std::endl;
	ldt::sdl_shutdown();

	return 0;
}
