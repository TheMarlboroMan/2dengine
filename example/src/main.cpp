#include "app/env.h"
#include "controller/controller_states.h"
#include "d2d/engine/main.h"
#include "dfwimpl/config.h"
#include "dfwimpl/state_driver.h"

int main(int argc, char ** argv)
{
	app::env env;
	
	//Init application log.
	//std::string log_path{env.get_app_path()+"logs/app.log"};
	//lm::file_logger log_app(log_path.c_str());
	lm::ostream_logger log_app(std::cout);

	int initial_state=controller::state_main;
	lm::log(log_app).info()<<"setting up config..."<<std::endl;
	dfwimpl::config config(env.get_usr_path());
	
	lm::log(log_app).info()<<"building state driver..."<<std::endl;
	dfwimpl::state_driver sd(config, log_app, env, initial_state);

	lm::log(log_app).info()<<"starting main process..."<<std::endl;
	return d2d::engine::main(argc, argv, sd, log_app);
}
