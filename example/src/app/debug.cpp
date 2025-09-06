#include <app/debug.h>

#ifdef IS_DEBUG_BUILD 

using namespace app;

lm::logger * app::debug::instance={nullptr};

#endif
