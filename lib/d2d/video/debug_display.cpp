#include "d2d/video/debug_display.h"
#include <ldv/box_representation.h>
#include <ldv/color.h>

using namespace d2d::video;

debug_display::debug_display(
	int _w,
	int _h
)
	:camera({0,0,(unsigned)_w,(unsigned)_h}, {0,0}),
	bgcolor{ldv::rgba_color(0,0,0,255)},
	outline{{0,0,0,0}, ldv::rgba_color(0,0,0,255), ldv::box_representation::type::line},
	fill{{0,0,0,0}, ldv::rgba_color(0,0,0,255), ldv::box_representation::type::fill}
{ 

	camera.set_coordinate_system(ldv::camera::tsystem::cartesian);
	fill.set_blend(ldv::representation::blends::alpha);
	outline.set_blend(ldv::representation::blends::alpha);
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
	int y=multiplier*(int)_item.get_y()-(int)_item.get_h();

	fill.set_location({(int)_item.get_x(), y, (unsigned)_item.get_w(), (unsigned)_item.get_h()});
	fill.set_color(_item.get_debug_fill_color());
	fill.draw(_screen, camera);

	outline.set_location({(int)_item.get_x(), y, (unsigned)_item.get_w(), (unsigned)_item.get_h()});
	outline.set_color(_item.get_debug_outline_color());
	outline.draw(_screen, camera);
}

void debug_display::draw(
	ldv::screen& _screen,
	const std::vector<d2d::collision::spatiable>& _items
) {

	for(const auto& item : _items) {

		draw(_screen, item);
	}
}

void debug_display::center_on(
	const d2d::collision::spatiable& _spatiable
) {
	int x=_spatiable.get_x()+(_spatiable.get_w() / 2);
	int y=_spatiable.get_y()-(_spatiable.get_h() / 2);

	camera.center_on({x, y});
}
