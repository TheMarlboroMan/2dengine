#pragma once

#include <dfw/state_driver_interface.h>
#include <lm/logger.h>
namespace d2d { namespace engine {

int main(
	int,
	char **,
	dfw::state_driver_interface&,
	lm::logger&
);
}}
