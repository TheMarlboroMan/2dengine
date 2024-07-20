#include "app/savegame_manager.h"
#include "app/savegame.h"
#include "app/types.h"
#include "app/definitions.h"

#include <filesystem>

using namespace app;

savegame_manager::savegame_manager(
	const appenv::env& _env
):
	env{_env}
{ }

void savegame_manager::load() {

	slots.clear();

	for(const auto& filename : {"savegame_1.dat", "savegame_2.dat", "savegame_3.dat"}) {

		const std::string path=env.build_user_path(filename);

		if(!std::filesystem::exists(path)) {

			slots.push_back({true, path, 0});
			continue;
		}

		//Read the savefile to be able to extract the collectibles...
		savegame_io sio{};
		auto save=sio.load_from_file(path);

		tpersistence persistence_temp;
		persistence_temp.load_from_string(save.persistence_string);

		//TODO: Shall we just save/read this from the inventory??
		//Filter the stuff in the collectible group that has 1 as a its value,
		//meaning it has been seen and picked up.
		int pickups=persistence_temp.size(
			app::pergr_collectibles,
			[](int _val) -> bool {return 1==_val;}
		);

		slots.push_back({false, path, pickups});
	}
}
