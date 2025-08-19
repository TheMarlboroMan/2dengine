#include <string>
#include <iostream>
namespace app { /**
 * structure that represents a saved game
 */
struct savegame {

	               savegame(
						const std::string&, //name
						const std::string&, //persistence
						int, //entry
						int, //skill
						int, //seconds
						int, //rooms
						int, //lives
						int, //keys
						int,
						int,
						int,
						int, //ultimate
						int //treasure
					);
	static savegame from_default();

	std::string map_filename,
	            persistence_string; //the persistence string as is, with newlines and all.
	int entry_id,
	    difficulty_setting,
	    elapsed_seconds,
	    discovered_rooms,
	    lives,
	    yellow_keys,
	    blue_keys,
	    red_keys,
	    green_keys,
	    treasure,
	    ultimate;

	bool is_malformed() const {

		return !map_filename.size()
			|| !persistence_string.size()
			|| discovered_rooms==-1
			|| entry_id==-1
			|| difficulty_setting==-1
			|| elapsed_seconds==-1
			|| lives==-1
			|| yellow_keys==-1
			|| blue_keys==-1
			|| red_keys==-1
			|| green_keys==-1
			|| treasure==-1
			|| ultimate==-1;
	}
};

class savegame_io {

	public:

	savegame        load_from_file(const std::string&);
	void            save_to_file(const std::string&, const savegame&);

	private:

	savegame        load(std::istream&);
	void            save(std::ostream&, const savegame&);
};
}
