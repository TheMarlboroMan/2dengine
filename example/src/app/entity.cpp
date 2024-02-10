#include "app/entity.h"

using namespace app;

entity::entity(
	int _x,
	int _y
):
	collision_box{{(double)_x, (double)_y}, 8, 24}
{}
