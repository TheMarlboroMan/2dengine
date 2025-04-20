#pragma once

#ifdef IS_DEBUG_BUILD 

#include <lm/logger.h>
#include <lm/log.h>
#include <sstream>

namespace app {

struct debug {

	static lm::logger * instance;
	static void         init(lm::logger& _logger) {instance=&_logger;}
	static lm::log      log() {return lm::log(*instance);}
};
}

#endif
