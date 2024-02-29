void main::console_display_onenter(
	const std::string& _command
) {

	console->send(_command);
}

console::result main::execute_cmd(
	const std::string& _cmd, 
	const std::vector<console::argument>& _args
) {

	if(_cmd=="exit") {

		console_enabled=false;
		return {0, "ok"};
	}

	if(_cmd=="tic") {

		tic(0.01f, app::player_input{});
		return {0, "ok"};
	}

	if(_cmd=="help") {

		std::stringstream ss;
		for(const auto& command : console->get_commands()) {

			ss<<command.name<<", ";
		}
		return {0, ss.str()};
	}

	if(_cmd=="helpcmd") {

		for(const auto& command : console->get_commands()) {

			if(command.name==_args[0].get_string()) {

				std::stringstream ss;
				ss<<command;
				return {0, ss.str()};
			}
		}

		return {1, "command not found"};
	}

	if(_cmd=="tell") {

		std::stringstream ss;
		ss<<player.ent;
		return{0, ss.str()};
	}	

	if(_cmd=="goto") {

		double x=_args[0].get_int(),
			y=_args[1].get_int();

		player.ent.set_origin({x, y});
		player.ent.sync_boxes();
		dd.center_on(player.ent);

		std::stringstream ss;
		ss<<"moved to "<<x<<","<<y;
		return {0, ss.str()};
	}

	if(_cmd=="debugdraw") {

		debug_draw=true;
		return {0, "debug draw enabled"};
	}

	if(_cmd=="gamedraw") {

		debug_draw=false;
		return {0, "debug draw disabled"};
	}

	if(_cmd=="moveby") {

		double x=_args[0].get_int(),
			y=_args[1].get_int();

		auto pt=player.ent.get_origin();
		pt+={x, y};

		player.ent.set_origin(pt);
		player.ent.sync_boxes();
		dd.center_on(player.ent);

		std::stringstream ss;
		ss<<"moved by "<<x<<","<<y;
		return {0, ss.str()};
	}

	if(_cmd=="get_collision_tiles") {

		d2d::collision::tiles_in_box adapter(shaper.get_tile_w(), shaper.get_tile_h());
		lm::log(logger).debug()<<"will debug the collisions\n";
		adapter.set_debug_enabled(true).set_logger(&logger);

		auto current_tiles=adapter.find(player.ent, current_map.tile_finder);

		if(!current_tiles.size()) {

			return {0, "no collisions with tiles, check log for details"};
		}

		std::stringstream ss;
		ss<<"total: "<<current_tiles.size()<<" check stdout and log for details";
		for(const auto& node : current_tiles) {

			std::cout<<*node<<", ";
		}

		std::cout<<std::endl;

		return {0, ss.str()};
	}

	return {0, "..."};
}

void main::setup_console(
	app::service_provider& _sp
) {

	std::function<void(const std::string&)> display_update=[&](const std::string& _cmd) -> void {

		console_display_onenter(_cmd);
	};

	console_display.reset(
		new appconsole::console{
			{0,0,480, 100},
			_sp.get_ttf_manager().get("console_font", 14),
			display_update
		}
	);

	console.reset(new console::console(*this));
	console->connect_output(console_display->get_output());

	//TODO: it would be nice to have some helper in the console library to 
	//read these from a file, or something.
	console->map_command("exit", {});
	console->map_command("help", {});
	console->map_command("helpcmd", {{console::types::string}});
	console->map_command("tic", {});
	console->map_command("tell", {});
	console->map_command("debugdraw", {});
	console->map_command("gamedraw", {});
	console->map_command("get_collision_tiles", {});
	console->map_command("goto", {{console::types::integer}, {console::types::integer}});
	console->map_command("moveby", {{console::types::integer}, {console::types::integer}});
}

void main::draw_debug(
	ldv::screen& _screen
) {

	dd.clear(_screen);

	for(const auto& cell : current_map.collision_tiles) {

		dd.draw(_screen, cell);
	}

	for(const auto& block : current_map.solid_blocks) {

		dd.draw(_screen, block);
	}

	for(const auto& block : current_map.platform_blocks) {

		dd.draw(_screen, block);
	}

	//TODO: Alpha is not working for these...
	for(const auto& ladder : current_map.ladders) {

		dd.draw(_screen, ladder);
	}

	dd.draw(_screen, player.ent);
}
