#include "app/secret_cover.h"

using namespace app;

secret_cover::secret_cover(
	d2d::collision::box _box, 
	int _id
):
	ent{_box},
	id{_id},
	timeout{2.5, 0., true}
{}

bool secret_cover::is_discovered() const {

	return states::discovered==state;
}

bool secret_cover::is_hidden() const {

	return states::hidden==state;
}

bool secret_cover::is_dissapearing() const {

	return states::dissapearing==state;
}

void secret_cover::tic(
	ldtools::tdelta _delta
) {

	timeout.tic(_delta);

	switch(state) {

		case states::hidden: return;
		case states::discovered: return;
		case states::dissapearing:

			if(is_dissapearing() && timeout.is_finished()) {

				state=states::discovered;
			}
			return;
	}
}

void secret_cover::discover() {

	state=states::dissapearing;
	timeout.resume();
}

std::ostream& app::operator<<(
	std::ostream& _stream, 
	const secret_cover& _cover
) {

	_stream<<"secret_cover["<<_cover.ent<<" id:"<<_cover.id<<"]";

	return _stream;
}

