//Generics.
#include "dfwimpl/state_driver.h"
#include "controller/controller_states.h"
#include <algorithm>
#include <filesystem>
#include <stdexcept>

#ifdef IS_DEBUG_BUILD
	#include "controller/test.h"
	#include <dfw/input_recorder_file_8bit.h>
	#include <dfw/input_generator_file_8bit.h>
#endif

//Libraries and application specifics.
#include <d2d/video/spritesheet_manager.h>
#include <d2d/video/animation_manager.h>
#include <ldtools/ttf_manager.h>
#include <ldtools/sprite_table.h>
#include <tools/i8n.h>
#include "app/input.h"
#include "app/definitions.h"

//Begin controllers
#include "controller/main.h"
#include "controller/menu.h"
#include "controller/pause.h"
#include "controller/controls.h"
#include "controller/options.h"
#include "controller/show_text.h"
//[new-controller-header-mark]

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

/**
 * We will have three blocks here. First are the most common things that we
 * should not fiddle too much with. Then the "config" / "behaviour" parts and 
 * finally the "application-specific" parts. These large comment banners 
 * separate each block.
 */

/**
 * These are sort of generic methods for most applications that get their 
 * data from elsewhere. We don't need to change much here.
 */

void state_driver::init(
	dfw::kernel& kernel
) {
	//Do some black magic.
	lm::log(log).info()<<"setting state check function..."<<std::endl;
	using std::placeholders::_1;
	std::function<bool(int)> vfunc=std::bind(&state_driver::validate_state, this, _1);
	states.set_function(vfunc);

	lm::log(log).info()<<"init state driver building: preparing video..."<<std::endl;
	bool fullscreen=config.bool_from_path("video:fullscreen");
	prepare_video(kernel, get_video_init_data(), fullscreen);

	lm::log(log).info()<<"preparing audio..."<<std::endl;
	prepare_audio(kernel);

	lm::log(log).info()<<"preparing input..."<<std::endl;
	prepare_input(kernel);

	lm::log(log).info()<<"preparing resources..."<<std::endl;
	prepare_resources(kernel);

	lm::log(log).info()<<"registering controllers..."<<std::endl;
	register_controllers(kernel);

	lm::log(log).info()<<"virtualizing input..."<<std::endl;
	virtualize_input(kernel.get_input(), get_input_axis_threshold());

	lm::log(log).info()<<"state driver fully constructed"<<std::endl;

	start_app(kernel.get_arg_manager(), kernel.get_input());
}

void state_driver::virtualize_input(
	dfw::input& input,
	int _threshold
) {

	lm::log(log).info()<<"trying to virtualize "<<input().get_joysticks_size()<<" controllers..."<<std::endl;

	for(size_t i=0; i < input().get_joysticks_size(); ++i) {

		input().virtualize_joystick_hats(i);
		input().virtualize_joystick_axis(i, _threshold);
		lm::log(log).info()<<"Joystick virtualized "<<i<<std::endl;
	}
}

void state_driver::prepare_video(
	dfw::kernel& kernel,
	dfw::window_info _window_info,
	bool _fullscreen
) {

	kernel.init_video_system(_window_info);
	kernel.get_screen().set_fullscreen(_fullscreen);
}

void state_driver::prepare_audio(dfw::kernel& kernel) {

	kernel.init_audio_system(get_audio_init_data());
}

void state_driver::prepare_input(dfw::kernel& kernel) {

	auto pairs=get_input_pairs();
	kernel.init_input_system(pairs);
}

void state_driver::prepare_resources(
	dfw::kernel& _kernel
) {

	load_resources(_kernel);
	ready_resources(_kernel);
}

/**
 * From here on these are "config" and "behaviour" like methods. Everything 
 * here could and should be changed to adapt to the application specific needs 
 * even if the structure can still be reused. Not everything here is a config, 
 * for example, load_resources can choose what kind of stuff to load.
 */
dfw::window_info state_driver::get_video_init_data() const {

	return {
		//This is the physical size...
		config.int_from_path("video:window_w_px"),
		config.int_from_path("video:window_h_px"),
		//And this is the LOGICAL size.
		app::logic_screen_w,
		app::logic_screen_h, 
		"---", //Title.
		false, //Show cursor.
		config.get_screen_vsync()
	};
}

dfw::audio_info state_driver::get_audio_init_data() const {

	return {
		config.get_audio_ratio(),
		config.get_audio_out(),
		config.get_audio_buffers(),
		config.get_audio_channels(),
		config.get_audio_volume(),
		config.get_music_volume()
	};
}

void state_driver::load_resources(
	dfw::kernel& _kernel
) {

	dfw::resource_loader r_loader(
		_kernel.get_video_resource_manager(), 
		_kernel.get_audio_resource_manager(), 
		env.build_app_path("")
	);

	//Textures.
	r_loader.generate_textures(tools::explode_lines_from_file(build_resource_path("lists", "textures.txt")));

	//Ssurfaces that may need to be loaded, for later manipulation into composite backgrounds.
	//r_loader.generate_surfaces(tools::explode_lines_from_file(build_resource_path("lists", "surfaces.txt")));
	
	//Sounds.
	r_loader.generate_sounds(tools::explode_lines_from_file(build_resource_path("lists", "sounds.txt")));
	
	//Music, unless it will be loaded and unloaded dynamically.
	//r_loader.generate_music(tools::explode_lines_from_file(build_resource_path("lists", "music.txt")));
}

void state_driver::common_pre_loop_input(
	dfw::input& input, 
	ldtools::tdelta /*delta*/
) {

	if(input().is_event_joystick_connected()) {

		lm::log(log).info()<<"New joystick detected..."<<std::endl;
		virtualize_input(input, get_input_axis_threshold());
	}
}

void state_driver::common_loop_input(dfw::input& /*input*/, ldtools::tdelta /*delta*/) {

}

void state_driver::common_pre_loop_step(ldtools::tdelta /*delta*/) {

}

void state_driver::common_loop_step(ldtools::tdelta /*delta*/) {

}

void state_driver::ready_resources(
	dfw::kernel& _kernel
) {
	//The service provider is a resource.
	service_provider.reset(
		new app::service_provider{env, config, log, _kernel}
	);

	std::string window_title=service_provider->get_localization().get("window-title");
	_kernel.get_screen().set_title(window_title);

	auto& spritesheets=service_provider->get_spritesheet_manager();
	spritesheets.add(
		app::ss_tiles,
		ldtools::sprite_table{build_resource_path("lists", "tiles.txt")}
	);

	auto& animations=service_provider->get_animation_manager();
	animations.add(
		app::animgr_tiles,
		ldtools::animation_table{ spritesheets.at(app::ss_tiles), build_resource_path("lists", "animations.txt")}
	);

	//Ready ttf fonts...
	auto &ttf_manager=service_provider->get_ttf_manager();
	load_fonts(ttf_manager);
}

void state_driver::load_fonts(
	ldtools::ttf_manager& _ttf_manager
) {

	std::ifstream font_stream{build_resource_path("lists", "fonts.txt")};
	std::string font_id, font_path;
	int font_size;

	while(true) {

		font_stream>>font_id>>font_size>>font_path;

		if(font_stream.eof()) {

			break;
		}

		_ttf_manager.insert(font_id, font_size, env.build_app_path(font_path));
	}
}

std::vector<dfw::input_pair> state_driver::get_input_pairs() const {

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

	std::vector<std::tuple<std::string, int>> controls={
		{"input:left", app::input::left},
		{"input:right", app::input::right},
		{"input:up", app::input::up},
		{"input:down", app::input::down},
		{"input:jump", app::input::jump},
		{"input:pause", app::input::pause}
#ifdef IS_DEBUG_BUILD
		,
		{"input:tic", app::input::tic},
		{"input:reload_values", app::input::reload_values}
#endif
	};

	for(const auto& pair : controls) {

		add(std::get<0>(pair), std::get<1>(pair));
	}

	return pairs;
}

int state_driver::get_input_axis_threshold() const {

	return 15000;
}

bool state_driver::validate_state(
	int _v
) {

	return _v > controller::state_min 
		&& _v < controller::state_max;
}


std::string state_driver::build_resource_path(
	const std::string _type,
	const std::string _str
) {

	std::stringstream ss;
	ss<<"resources/"<<type<<"/"<<_str;
	return env.build_app_path(ss.str())};
};


/**
 * And here are the "application-specific" pieces. The structure of 
 * register_controllers is likely to be the same but both prepare_state 
 * and start_app can be trashed and started anew.
 */

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

	reg(
		c_options,
		controller::state_options,
		new controller::options(*service_provider)
	);

	reg(
		c_show_text,
		controller::state_show_text,
		new controller::show_text(*service_provider)
	);

#ifdef IS_DEBUG_BUILD

	reg(
		c_test,
		controller::state_test,
		new controller::test(*service_provider)
	);

#endif
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

void state_driver::start_app(

#ifdef IS_DEBUG_BUILD
	const tools::arg_manager& _argman,
	dfw::input& _in
#else
	const tools::arg_manager&,
	dfw::input&
#endif
) {

#ifdef IS_DEBUG_BUILD

	if(_argman.exists("--map")) {

		if(!_argman.arg_follows("--map")) {

			throw std::runtime_error("an argument must follow --map");
		}

		auto& gs=service_provider->get_game_session();

		if(_argman.exists("--skill")) {

			if(!_argman.arg_follows("--skill")) {

				throw std::runtime_error("an argument must follow --skill (1, 2, 3)");
			}

			int skill=std::stoi(_argman.get_following("--skill"));

			switch(skill) {

				case 1:
					gs.set_skill_level(app::skill_easy); 
					lm::log(log).info()<<"skill set to easy"<<std::endl;
				break;
				case 2:
					gs.set_skill_level(app::skill_normal); 
					lm::log(log).info()<<"skill set to normal"<<std::endl;
				break;
				case 3:
					gs.set_skill_level(app::skill_hard); 
					lm::log(log).info()<<"skill set to hard"<<std::endl;
				break;
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

		states.push(controller::states::state_main);

		int entry_id=1;

		if(_argman.exists("--eid")) {

			if(!_argman.arg_follows("--eid")) {

				throw std::runtime_error("an argument must follow --eid with the entry id");
			}

			entry_id=std::stoi(_argman.get_following("--eid"));
		}

		auto& mainc=static_cast<controller::main&>(*c_main);
		mainc.start(_argman.get_following("--map"), entry_id);
	}

	if(_argman.exists("--record")) {

		if(!_argman.arg_follows("--record")) {

			throw std::runtime_error("--record must be followed by filename");
		}

		auto recorded_filename=_argman.get_following("--record");
		if(std::filesystem::exists(recorded_filename)) {

			throw std::runtime_error("file for --record already exists, refusing to overwrite");
		}

		dfw::input_recorder_file_8bit * ir=new dfw::input_recorder_file_8bit{_in, input_converter};
		ir->open_file(recorded_filename);
		ir->set_active(true);
		ir->set_inputs({
			app::input::escape,
			app::input::left,
			app::input::right,
			app::input::up,
			app::input::down,
			app::input::jump,
			app::input::pause
		});

		input_recorder.reset(ir);
//TODO: Should be checked in dfw::input to see if any inputs are being watched.
		_in.set_recorder(input_recorder.get());

		lm::log(log).info()<<"inputs will be recorded to recorded-play"<<std::endl;
	}
	else if(_argman.exists("--replay")) {

		if(!_argman.arg_follows("--replay")) {

			throw std::runtime_error("--replay must be followed by filename");
		}

		auto recorded_filename=_argman.get_following("--replay");
		if(!std::filesystem::exists(recorded_filename)) {

			throw std::runtime_error("file for --replay not found");
		}

		dfw::input_generator_file_8bit * ig=new dfw::input_generator_file_8bit{input_converter};
		ig->set_active(true);
		ig->open_file(recorded_filename);

		input_generator.reset(ig);
		_in.set_generator(input_generator.get());

		lm::log(log).info()<<"inputs will be replayed from recorded-play"<<std::endl;
	}

#endif
}

