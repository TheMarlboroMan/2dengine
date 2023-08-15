#include "d2d/video/debug_display.h"
#include <ldv/box_representation.h>
#include <ldv/color.h>

using namespace d2d::video;

debug_display::debug_display(
	int _w,
	int _h
)
	:camera({0,0,(unsigned)_w,(unsigned)_h}, {0,0}),
	bgcolor{ldv::rgba_color(0,0,0,255)}
{ 

	camera.set_coordinate_system(ldv::camera::tsystem::cartesian);
}

void debug_display::clear(
	ldv::screen& _screen
) {

	_screen.clear(bgcolor);
}

void debug_display::draw(
	ldv::screen& _screen,
	const d2d::collision::spatiable& _item
) {

	int multiplier=-1; //see camera.h, we are using a cartesian system. we must do this.

	//TODO: These should be properties.
	ldv::box_representation outline({0,0,0,0}, ldv::rgba_color(0,0,0,0), ldv::box_representation::type::line);
	ldv::box_representation fill({0,0,0,0}, ldv::rgba_color(0,0,0,0), ldv::box_representation::type::fill);

	fill.set_location({_item.get_x(), multiplier*_item.get_y(), _item.get_w(), _item.get_h()});
	fill.draw(_screen, camera);

	outline.set_location({_item.get_x(), multiplier*-_item.get_y(), _item.get_w(), _item.get_h()});
	outline.draw(_screen, camera);
}

void debug_display::draw(
	ldv::screen& _screen,
	const std::vector<d2d::collision::spatiable>& _items
) {

	int multiplier=-1; //see camera.h, we are using a cartesian system. we must do this.

	ldv::box_representation outline({0,0,0,0}, ldv::rgba_color(0,0,0,0), ldv::box_representation::type::line);
	ldv::box_representation fill({0,0,0,0}, ldv::rgba_color(0,0,0,0), ldv::box_representation::type::fill);

	for(const auto& item : _items) {

		//TODO: fix narrowing...
		fill.set_location({item.get_x(), multiplier*-item.get_y(), item.get_w(), item.get_h()});
		fill.draw(_screen, camera);

		outline.set_location({item.get_x(), multiplier*-item.get_y(), item.get_w(), item.get_h()});
		outline.draw(_screen, camera);
	}
}

void debug_display::center_on(
	const d2d::collision::spatiable& _spatiable
) {
	int x=_spatiable.get_x()+(_spatiable.get_w() / 2);
	int y=_spatiable.get_y()-(_spatiable.get_h() / 2);

	camera.center_on({x, y});
}
