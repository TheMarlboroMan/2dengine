#include "app/secret_cover.h"

using namespace app;

secret_cover::secret_cover(
	d2d::collision::box _box, 
	int _id
):
	ent{_box},
	id{_id}
{}

std::ostream& app::operator<<(
	std::ostream& _stream, 
	const secret_cover& _cover
) {

	_stream<<"secret_cover["<<_cover.ent<<" id:"<<_cover.id<<"]";

	return _stream;
}
