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

#else
		appenv::env(std::string{".the_dreaming_tower"}, &_logger) 
#endif
	{}
};

}
