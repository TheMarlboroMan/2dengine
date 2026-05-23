#include <app/thing_filter.h>
#include <d2d/collision/tools.h>

using namespace app;

thing_filter_moving_block::thing_filter_moving_block(
	const d2d::collision::box& _ref,
	bool _positive_y_vector
):ref_box(_ref), positive_y_vector{_positive_y_vector}
{}

bool thing_filter_moving_block::operator()(
	const app::moving_block& _block
) const {

	if(_block.is_solid()) {

		return true;
	}

	//Non solid blocks are non-existent if there is a positive y vector.
	if(positive_y_vector) {

		return false;
	}

	//Count only those above the reference box. This takes care of
	//semi transparent blocks moving sideways towards a player and
	//not pushing them.
	return d2d::collision::is_above(_block.ent, ref_box);
}

