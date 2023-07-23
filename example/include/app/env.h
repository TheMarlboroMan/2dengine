#pragma once

#include <string>
#include <stdlib.h>

namespace app {

class env {

	public:

	std::string              get_app_path() const {return std::string{"./"};}
	std::string              get_usr_path() const {return std::string{"./"};}
};

}
