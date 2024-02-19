#pragma once

#include <appenv/env.h>
#include <string>
#include <stdlib.h>

namespace app {

class env:
	public appenv::env {

	public:

	                        env(lm::logger& _logger):
		appenv::env(std::string{".d2d-example-app"}, &_logger) 
	{}

	std::string              get_app_path() const {return std::string{"./"};}
	std::string              get_usr_path() const {return std::string{"./"};}
};

}
