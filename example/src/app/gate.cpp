#include "app/gate.h"
#include "app/definitions.h"

using namespace app;

gate::gate(
	d2d::collision::box _box,
	int _tag
):
	ent{_box},
	tag{_tag},
	state{states::closed},
	destination_y{(int)_box.origin.y+_box.h-y_threshold}
{}

void gate::tic(
	ldtools::tdelta _delta
) {

	switch(state) {

		case states::open:
		case states::closed:
			return;

		case states::opening:{

			double y=ent.get_y()+(_delta * 16.);
			ent.set_y(y);
			ent.commit_box();
			if(y >= destination_y) {

				open();
			}
		}
		break;
	}
}

void gate::activate() {

	if(state==states::closed) {

		state=states::opening;
	}
}

void gate::open() {

	ent.set_y(destination_y);
	ent.commit_box();
	state=states::open;
}

bool gate::is_closed() const {

	return states::closed==state;
}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const gate& _gate
) {

	_stream<<"gate["<<_gate.ent<<" tag:"<<_gate.tag<<"]";
	return _stream;
}

