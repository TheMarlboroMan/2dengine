#include "app/service_provider.h"

using namespace app;

service_provider::service_provider(
	const app::env& _env, 
	const dfwimpl::config& _config,
	lm::logger& _logger
):
	env{_env},
	config{_config},
	logger{_logger}
{ }

d2d::collision::shaper& service_provider::get_shaper() {

	if(nullptr==shaper.get()) {

		shaper.reset(new d2d::collision::shaper(24, 24));
	}

	return *shaper;
}

const app::tile_impl& service_provider::get_tile_impl() {

	if(nullptr==tile_impl.get()) {

		tile_impl.reset(new app::tile_impl());
	}

	return *tile_impl;
}

