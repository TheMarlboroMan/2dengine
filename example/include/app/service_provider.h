#pragma once

#include <memory>
//Sadly a forward declaration it not enough for the unique_ptr...
#include <d2d/collision/shaper.h>
#include "app/tile_impl.h"

namespace lm {
class logger;
}

namespace dfwimpl{
class config;
}
/**
 
namespace d2d{namespace collision{
class shaper;
}};
*/

namespace app {

class env;

class service_provider {

	public:

	service_provider(const app::env&, const dfwimpl::config&, lm::logger&);

	const app::env&			get_env() const {return env;}
	const dfwimpl::config&	get_config() const {return config;}
	lm::logger&				get_logger() {return logger;}
	d2d::collision::shaper& get_shaper();
	const app::tile_impl&   get_tile_impl();

	private:

	const app::env&			env;
	const dfwimpl::config&	config;
	lm::logger&				logger;
	std::unique_ptr<d2d::collision::shaper>	shaper{nullptr};
	std::unique_ptr<app::tile_impl>	tile_impl{nullptr};
};
}
