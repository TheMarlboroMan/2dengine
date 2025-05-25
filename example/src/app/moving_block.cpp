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
	velocity{0., 40.},
	type{_t}
{

	if(type==2) {

		velocity={20., 0.};
	}
}

void moving_block::tic(
	ldtools::tdelta _delta
) {

	//TODO: the mover should be passed by ref.
	d2d::motion::mover m{};
	m.apply(ent, velocity, _delta);

	switch(type) {

		case 1:

			if(ent.get_y() >= 12*16) {

				velocity.y*=-1.;
			}
			else if(ent.get_y() <= 0) {

				velocity.y*=-1.;
			}
		break;
		case 2:

			if(ent.get_x() >= 12*16) {

				velocity.x*=-1.;
			}
			else if(ent.get_x() <= 6*16) {

				velocity.x*=-1.;
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
