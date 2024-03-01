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

	for(const auto& node : current_map.entries) {

		dd.draw(_screen, node.ent);
	}

	for(const auto& node : current_map.exits) {

		dd.draw(_screen, node.ent);
	}

	dd.draw(_screen, player.ent);
}

void main::loop_debug(
	dfw::input& _input,
	const dfw::loop_iteration_data& _lid
) {

	if(_input().is_exit_signal()) {
		set_leave(true);
		return;
	}

	app::player_input pli{};
	if(console_enabled) {

		_input().start_text_input();
		console_display->input(_input());
		return;
	}

	if(_input.is_input_down(app::input::escape)) {

		set_leave(true);
		return;
	}

	if(_input.is_input_down(app::input::tic)) {

		console_enabled=true;
		return;
	}

	if(_input.is_input_down(app::input::reload_values)) {

		reload_values();
	}

	if(_input.is_input_pressed(app::input::down)) {

		pli.y=-1;
	}
	else if(_input.is_input_pressed(app::input::up)) {

		pli.y=1;
		if(_input.is_input_down(app::input::up)) {

			pli.enter_door=true;
		}
	}

	if(_input.is_input_pressed(app::input::jump)) {

		pli.hold_jump=true;

		if(_input.is_input_down(app::input::jump)) {

			pli.jump=true;
		}
	}

	if(_input.is_input_pressed(app::input::left)) {

		pli.x=-1;
	}
	else if(_input.is_input_pressed(app::input::right)) {

		pli.x=1;
	}

	tic(_lid.delta, pli);
}


void main::reload_values() {

	std::cout<<current_map<<std::endl;
	
	std::stringstream ss;
	ss<<env.get_app_path()+"resources/runtime/values";
	std::cout<<"values file will be read from "<<ss.str()<<std::endl;

	std::ifstream values_file{ss.str().c_str()};

	if(!values_file.is_open()) {

		throw std::runtime_error("could not open values file!");
	}

	while(true) {

		std::string line;
		std::getline(values_file, line);

		if(values_file.eof()) {

			break;
		}

		const auto pieces=tools::explode(line, '=', 2);
		if(2 != pieces.size()) {

			continue;
		}

		const auto& name=pieces[0];
		const auto& value=pieces[1];

		if(name=="gravity_force_x") {

			simulation.gravity.force.x=std::stod(value);
		}
		else if(name=="gravity_force_y") {

			simulation.gravity.force.y=std::stod(value);
		}
		else if(name=="gravity_max_velocity") {

			simulation.gravity.max_velocity=std::stod(value);
		}
		else if(name=="jump_force") {

			simulation.jump_force=std::stod(value);
		}
		else if(name=="walk_max_velocity") {

			simulation.walk_max_velocity=std::stod(value);
		}
		else if(name=="ladder_max_velocity") {

			simulation.ladder_max_velocity=std::stod(value);
		}
		else if(name=="air_x_velocity_reduce_factor") {

			simulation.air_x_velocity_reduce_factor=std::stod(value);
		}
		else if(name=="air_x_velocity_acceleration_value") {

			simulation.air_x_velocity_acceleration_value=std::stod(value);
		}
		else if(name=="air_x_velocity_collision_reduce_factor") {

			simulation.air_x_velocity_collision_reduce_factor=std::stod(value);
		}
		else if(name=="air_y_velocity_jump_shorten_factor") {

			simulation.air_y_velocity_jump_shorten_factor=std::stod(value);
		}
		else if(name=="defeat_y_velocity") {

			simulation.defeat_y_velocity=std::stod(value);
		}
		else {

			std::cout<<"bad name: "<<name<<std::endl;
		}
	}
}
