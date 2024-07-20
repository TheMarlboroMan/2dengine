#include <string>
#include <iostream>
namespace app { /**
 * structure that represents a saved game
 */
struct savegame {

	std::string map_filename{""},
	            persistence_string{""}; //the persistence string as is, with newlines and all.
	int entry_id{-1},
	    difficulty_setting{-1},
	    elapsed_seconds{-1},
	    lives{-1},
	    yellow_keys{-1},
	    blue_keys{-1},
	    red_keys{-1},
	    green_keys{-1};

	bool is_malformed() const {

		return !map_filename.size()
			|| !persistence_string.size()
			|| entry_id==-1
			|| difficulty_setting==-1
			|| elapsed_seconds==-1
			|| lives==-1
			|| yellow_keys==-1
			|| blue_keys==-1
			|| red_keys==-1
			|| green_keys==-1;
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
