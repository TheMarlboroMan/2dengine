#pragma once
#include <appenv/env.h>
#include <string>
#include <stdlib.h>

namespace app {

class env:
	public appenv::env {

	public:

	                        env(lm::logger& _logger):
//TODO: This should have a name, right?
		appenv::env(std::string{".d2d-example-app"}, &_logger) 
	{}
};

}
