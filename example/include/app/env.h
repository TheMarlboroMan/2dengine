#pragma once
#include <appenv/env.h>
#include <string>
#include <stdlib.h>

namespace app {

class env:
	public appenv::env {

	public:


	                        env(lm::logger& _logger):
#ifdef WINBUILD
		appenv::env(std::string{"user_data"}, &_logger) 
#else
		appenv::env(std::string{".the_dreaming_tower"}, &_logger) 
#endif
	{}
};

}
