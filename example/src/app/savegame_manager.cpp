#include "app/savegame_manager.h"
#include "app/savegame.h"
#include "app/types.h"
#include "app/definitions.h"

#include <filesystem>
#include <stdexcept>

using namespace app;

savegame_manager::savegame_manager(
	const appenv::env& _env,
	const std::string& _affix
):
	env{_env},
	savegame_affix{_affix}
{ }

const save_slot& savegame_manager::get(
	std::size_t _index
) const {

	if(_index >= slots.size()) {

		throw std::runtime_error("out of bounds slot");
	}

	return slots.at(_index);
}

void savegame_manager::load() {

	slots.clear();


	std::vector<std::string> filenames={
		"savegame_"+savegame_affix+"_1.dat",
		"savegame_"+savegame_affix+"_2.dat",
		"savegame_"+savegame_affix+"_3.dat"
	};

	for(const auto& filename : filenames) {

		const std::string path=env.build_user_path(filename);

		if(!std::filesystem::exists(path)) {

			slots.push_back({true, path, "", 0, 0, 0});
			continue;
		}

		//Read the savefile to be able to extract the collectibles...
		savegame_io sio{};
		auto save=sio.load_from_file(path);

		tpersistence persistence_temp;
		persistence_temp.load_from_string(save.persistence_string);

		//Filter the stuff in the collectible group that has 1 as a its value,
		//meaning it has been seen and picked up.
		int pickups=persistence_temp.size(
			app::pergr_collectibles,
			[](int _val) -> bool {return 1==_val;}
		);

		slots.push_back({false, path, save.map_filename, pickups, save.difficulty_setting, save.elapsed_seconds});
	}
}

void savegame_manager::erase(
	std::size_t _slot_index
) {

	if(_slot_index >= slots.size()) {

		throw std::runtime_error("out of bounds slot");
	}

	const auto& slot=slots[_slot_index];
	if(slot.new_game) {

		throw std::runtime_error("will not erase non-existing slot");
	}

	std::filesystem::remove(slot.filename);
	load();
}

