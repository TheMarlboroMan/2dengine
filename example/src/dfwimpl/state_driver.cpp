#include "dfwimpl/state_driver.h"
#include "app/input.h"
#include "app/definitions.h"
#include "controller/controller_states.h"

#include <d2d/video/spritesheet_manager.h>
#include <d2d/video/animation_manager.h>
#include <ldtools/ttf_manager.h>
#include <ldtools/sprite_table.h>
#include <tools/i8n.h>
#include <algorithm>
#include <filesystem>

#include "controller/main.h"
#include "controller/menu.h"
#include "controller/pause.h"
#include "controller/controls.h"
//[new-controller-header-mark]
//
using namespace dfwimpl;

state_driver::state_driver(
	dfwimpl::config& c,
	lm::logger& _logger,
	const appenv::env& _env,
	int _initial_state
):
	state_driver_interface(_initial_state),
	config(c),
	log{_logger},
	env{_env}
{ }

void state_driver::init(
	dfw::kernel& kernel
) {

	lm::log(log).info()<<"setting state check function..."<<std::endl;
	states.set_function([](int v){
		return v > controller::state_min && v < controller::state_max;
	});

	lm::log(log).info()<<"init state driver building: preparing video..."<<std::endl;
	prepare_video(kernel);

	lm::log(log).info()<<"preparing audio..."<<std::endl;
	prepare_audio(kernel);

	lm::log(log).info()<<"preparing input..."<<std::endl;
	prepare_input(kernel);

	lm::log(log).info()<<"preparing resources..."<<std::endl;
	prepare_resources(kernel);
	load_resources();

	lm::log(log).info()<<"registering controllers..."<<std::endl;
	register_controllers(kernel);

	lm::log(log).info()<<"virtualizing input..."<<std::endl;
	virtualize_input(kernel.get_input());

	lm::log(log).info()<<"state driver fully constructed"<<std::endl;

	start_app(kernel.get_arg_manager());
}

void state_driver::prepare_video(dfw::kernel& kernel) {

	kernel.init_video_system({
		//This is the physical size...
		config.int_from_path("video:window_w_px"),
		config.int_from_path("video:window_h_px"),
		//And this is the LOGICAL size.
		app::logic_screen_w,
		app::logic_screen_h, 
		"Title for this project",
		false,
		config.get_screen_vsync()
	});

	auto& screen=kernel.get_screen();
	screen.set_fullscreen(config.bool_from_path("video:fullscreen"));
}

void state_driver::prepare_audio(dfw::kernel& kernel) {

	kernel.init_audio_system({
		config.get_audio_ratio(),
		config.get_audio_out(),
		config.get_audio_buffers(),
		config.get_audio_channels(),
		config.get_audio_volume(),
		config.get_music_volume()
	});
}

void state_driver::prepare_input(dfw::kernel& kernel) {

	using namespace dfw;

	std::vector<input_pair> pairs{
		{{input_description::types::keyboard, SDL_SCANCODE_ESCAPE, 0}, app::input::escape}
	};

	auto add=[&](std::string _token, int _input_type) {

		for(const auto desc : input_description_from_config_token(config.token_from_path(_token))) {

			lm::log(log).info()<<"setting input for "<<_input_type<<" as code "<<desc.code<<std::endl;
			pairs.push_back({desc, _input_type});
		}
	};

	add("input:left", app::input::left);
	add("input:right", app::input::right);
	add("input:up", app::input::up);
	add("input:down", app::input::down);
	add("input:jump", app::input::jump);
	add("input:pause", app::input::pause);
	add("input:tic", app::input::tic);
	add("input:reload_values", app::input::reload_values);

	kernel.init_input_system(pairs);
}

void state_driver::prepare_resources(
	dfw::kernel& _kernel
) {

	dfw::resource_loader r_loader(_kernel.get_video_resource_manager(), _kernel.get_audio_resource_manager(), env.build_app_path(""));

	r_loader.generate_textures(tools::explode_lines_from_file(env.build_app_path("resources/lists/textures.txt")));

	//Some surfaces need to be loaded, for later manipulation into composite backgrounds.
	//r_loader.generate_surfaces(tools::explode_lines_from_file(env.build_app_path("data/lists/surfaces.txt"));
	//
	r_loader.generate_sounds(tools::explode_lines_from_file(env.build_app_path("resources/lists/sounds.txt")));
	//music will be dynamically loaded as needed, so there.
	//r_loader.generate_music(tools::explode_lines_from_file(env.build_app_path("resources/lists/music.txt")));
	
	service_provider.reset(
		new app::service_provider{env, config, log, _kernel}
	);

	auto& persistence=service_provider->get_persistence();
	persistence.add(app::pergr_collectibles);
	persistence.add(app::pergr_secret_covers);
	persistence.add(app::pergr_buttons);
	persistence.add(app::pergr_touch_triggers);
	persistence.add(app::pergr_automap);

	_kernel.get_screen().set_title(
		service_provider->get_localization().get("window-title")
	);
}

void state_driver::register_controllers(
	dfw::kernel& 
) {

	auto reg=[this](ptr_controller& _ptr, int _i, dfw::controller_interface * _ci) {

		_ptr.reset(_ci);
		register_controller(_i, *_ptr);
	};

	reg(
		c_main,
		controller::state_main,
		new controller::main(*service_provider)
	);

	reg(
		c_menu,
		controller::state_menu,
		new controller::menu(*service_provider)
	);

	reg(
		c_pause,
		controller::state_pause,
		new controller::pause(*service_provider)
	);

	reg(
		c_controls,
		controller::state_controls,
		new controller::controls(*service_provider)
	);
}

void state_driver::prepare_state(
	int _next,
	int _current
) {

	lm::log(log).info()<<"prepare state from "<<_current<<" to "<<_next<<std::endl;

	if(_current==controller::state_menu && _next==controller::state_main) {

		//Are we starting a new game?
		auto& menuc=static_cast<controller::menu&>(*c_menu);
		auto& mainc=static_cast<controller::main&>(*c_main);

		switch(menuc.get_action_signal()) {

			case controller::menu::signals::new_game:

				lm::log(log).info()<<"is new game, will reset everything"<<std::endl;

				mainc.new_game(
					menuc.get_selected_skill(),
					menuc.get_slot_filename()
				);
				menuc.set_can_continue();
				return;

			case controller::menu::signals::continue_game:

				lm::log(log).info()<<"game will resume"<<std::endl;
				return;

			case controller::menu::signals::load_game:

				lm::log(log).info()<<"is load game, will instruct to load the savegame"<<std::endl;
				service_provider->get_game_session().reset( //reset, data will be loaded.
					menuc.get_selected_skill(),
					menuc.get_slot_filename()
				);
				mainc.load_game();
				menuc.set_can_continue();
				return;
		}
	}
}

void state_driver::common_pre_loop_input(dfw::input& input, float /*delta*/) {

	if(input().is_event_joystick_connected()) {

		lm::log(log).info()<<"New joystick detected..."<<std::endl;
		virtualize_input(input);
	}
}

void state_driver::common_loop_input(dfw::input& /*input*/, float /*delta*/) {

}

void state_driver::common_pre_loop_step(float /*delta*/) {

}

void state_driver::common_loop_step(float /*delta*/) {

}

void state_driver::virtualize_input(dfw::input& input) {

	lm::log(log).info()<<"trying to virtualize "<<input().get_joysticks_size()<<" controllers..."<<std::endl;

	for(size_t i=0; i < input().get_joysticks_size(); ++i) {

		input().virtualize_joystick_hats(i);
		input().virtualize_joystick_axis(i, 15000);
		lm::log(log).info()<<"Joystick virtualized "<<i<<std::endl;
	}
}

void state_driver::start_app(
	const tools::arg_manager& _argman
) {

#ifdef IS_DEBUG_BUILD

	if(_argman.exists("--map")) {

		auto& gs=service_provider->get_game_session();

		if(_argman.exists("--skill")) {

			int skill=std::stoi(_argman.get_following("--skill"));

			switch(skill) {

				case 1: gs.set_skill_level(app::skill_easy); break;
				case 2: gs.set_skill_level(app::skill_normal); break;
				case 3: gs.set_skill_level(app::skill_hard); break;
				default:
					gs.set_skill_level(app::skill_normal);
					lm::log(log).notice()<<"skill set to normal, possible values are 1=easy, 2=normal and 3=hard"<<std::endl;
				break;
			}
		}
		else {

			gs.set_skill_level(app::skill_normal);
			lm::log(log).notice()<<"skill set to normal by default, use --skill to choose, possible values are 1=easy, 2=normal and 3=hard"<<std::endl;
		}

		states.set(controller::states::state_main);

		int entry_id=1;

		if(_argman.exists("--eid")) {

			entry_id=std::stoi(_argman.get_following("--eid"));
		}

		auto& mainc=static_cast<controller::main&>(*c_main);
		mainc.start(_argman.get_following("--map"), entry_id);
	}

#endif
/*
#ifndef NDEBUG
	if(_argman.exists("-s")) {

		static_cast<controller::main&>(*c_main).debug_start_step_control();
	}

#endif
*/
}

void state_driver::load_resources() {

	auto& spritesheets=service_provider->get_spritesheet_manager();
	spritesheets.add(
		app::ss_tiles, 
		ldtools::sprite_table{env.build_app_path("resources/lists/tiles.txt")}
	);

	auto& animations=service_provider->get_animation_manager();
	animations.add(
		app::animgr_tiles,
		ldtools::animation_table{ spritesheets.at(app::ss_tiles), env.build_app_path("resources/lists/animations.txt")}
	);

	auto &ttf_manager=service_provider->get_ttf_manager();
	ttf_manager.insert(
		"console_font",
		8,
		env.build_app_path("resources/fonts/publicpixel.ttf")
	);

	ttf_manager.insert(
		"pause_font",
		8,
		env.build_app_path("resources/fonts/publicpixel.ttf")
	);

	ttf_manager.insert(
		"menu_font",
		16,
		env.build_app_path("resources/fonts/publicpixel.ttf")
	);

	ttf_manager.insert(
		"menu_font",
		8,
		env.build_app_path("resources/fonts/publicpixel.ttf")
	);

	ttf_manager.insert(
		"area_banner_font",
		8,
		env.build_app_path("resources/fonts/publicpixel.ttf")
	);

	ttf_manager.insert(
		"lives_banner_font",
		8,
		env.build_app_path("resources/fonts/publicpixel.ttf")
	);
}
