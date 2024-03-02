#pragma once

#include <stdexcept>
#include <string>

namespace d2d {namespace components {

class exception:
	public std::runtime_error {

	public:
		exception(const std::string& _err)
			:std::runtime_error(_err) 
	{}
};
}}
