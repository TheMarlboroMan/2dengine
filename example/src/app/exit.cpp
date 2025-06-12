#include "app/exit.h"

using namespace app;

exit::exit(
	d2d::collision::box _box, 
	const std::string& _map_filename, 
	int _next_entry_id, 
	int _min_rooms, 
	bool _touch
):
	ent{_box},
	map_filename{_map_filename},
	next_entry_id{_next_entry_id},
	min_rooms{_min_rooms},
	touch{_touch}
{}


std::ostream& app::operator<<(
	std::ostream& _stream, 
	const exit& _exit
) {

	_stream<<"exit["<<_exit.ent.get_box()<<" next_id:"<<_exit.next_entry_id<<" name:"<<_exit.map_filename;
	return _stream;
}


