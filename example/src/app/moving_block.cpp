#include "app/moving_block.h"
#include <d2d/motion/mover.h>

using namespace app;

moving_block::moving_block(
	int _x,
	int _y,
	int _w,
	int _h,
	int _t
):
	ent{{(double)_x, (double)_y}, _w, _h},
	type{_t}
{

	ent.set_motion_vector({0., 40.});

	if(type==2) {

		ent.set_motion_vector({20., 0.});
	}
}

void moving_block::reset() {

	//TODO: Set this in its original state!
}

void moving_block::tic(
	ldtools::tdelta _delta
) {

	//TODO: the mover should be passed by ref.
	d2d::motion::mover m{};
	m.apply(ent, _delta);

	switch(type) {

		case 1:

			if(ent.get_y() >= 8*16) {

				ent.set_motion_vector_y(ent.get_motion_vector_y()*-1);
			}
			else if(ent.get_y() <= 0) {

				ent.set_motion_vector_y(ent.get_motion_vector_y()*-1);
			}
		break;
		case 2:

			if(ent.get_x() >= 12*16) {

				ent.set_motion_vector_x(ent.get_motion_vector_x()*-1);
			}
			else if(ent.get_x() <= 6*16) {

				ent.set_motion_vector_x(ent.get_motion_vector_x()*-1);
			}
		break;
	}
}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const moving_block& _block
) {

	_stream<<"moving_block["<<_block.ent.get_box()<<"]";
	return _stream;
}
