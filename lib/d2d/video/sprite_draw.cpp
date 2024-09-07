#include "d2d/video/sprite_draw.h"
#include "d2d/video/exception.h"
#include "d2d/video/tools.h"
#include <iostream>

using namespace d2d::video;

sprite_draw::sprite_draw(
	const ldtools::sprite_table& _sprite_table,
	const ldv::texture& _texture,
	ldv::camera * _camera,
	bool _with_camera
): 
	bmp{_texture},
	sprite_table{&_sprite_table},
	camera{_camera},
	with_camera{_with_camera}
{

	bmp.set_blend(ldv::representation::blends::alpha);
}

sprite_draw& sprite_draw::reset_sprite_table(
	const ldtools::sprite_table& _table
) {

	sprite_table=&_table;
	return *this;
}

sprite_draw& sprite_draw::reset_texture(
	const ldv::texture& _texture
) {

	bmp.set_texture(_texture);
	return *this;
}

//Versions with point and camera.

void sprite_draw::draw(
	ldv::screen& _screen,
	ldv::point _point,
	const ldtools::sprite_frame& _frame,
	sprite_draw::modifiers _modifiers
) {

	draw(_screen, {_point, _frame.box.w, _frame.box.h}, _frame, _modifiers);
}

void sprite_draw::draw(
	ldv::screen& _screen,
	ldv::point _point,
	const ldtools::sprite_frame& _frame
) {

	int flags=0;
	if(_frame.flags & ldtools::sprite_frame::horizontal_flip) {

		flags &= sprite_draw::modifiers::flip_horizontal;
	}

	if(_frame.flags & ldtools::sprite_frame::vertical_flip) {

		flags &= sprite_draw::modifiers::flip_vertical;
	}

	draw(
		_screen, _point, _frame, 
		{
			flags,
			_frame.get_rotation()
		}
	);
}

void sprite_draw::draw(
	ldv::screen& _screen, 
	ldv::point _point, 
	int _index,
	sprite_draw::modifiers _modifiers
) {

	const auto& frame=sprite_table->get(_index);
	draw(_screen, _point, frame, _modifiers);
}

void sprite_draw::draw(
	ldv::screen& _screen, 
	ldv::point _point, 
	int _index
) {

	const auto& frame=sprite_table->get(_index);
	draw(_screen, _point, frame);
}

//Versions with rect and camera.

void sprite_draw::draw(
	ldv::screen& _screen,
	ldv::rect _rect,
	const ldtools::sprite_frame& _frame,
	sprite_draw::modifiers _modifiers
) {
	//This is the one method the rest end up in. Assume all sprites are 

	bmp.set_clip({{_frame.box.origin.x, _frame.box.origin.y}, _frame.box.w, _frame.box.h});

	auto pt=d2d::video::to_screen_coordinates(_rect.origin, _rect.h);

	bool invert_horizontal=_modifiers.flags & modifiers::flip_horizontal;
	bool invert_vertical=_modifiers.flags & modifiers::flip_vertical;

	//Displacement will be used as top-left corner.
	pt.x+=(invert_horizontal ? 1 : -1) * _frame.disp_x; 
	pt.y+=(invert_vertical ? 1 : -1) * _frame.disp_y;
	_rect.origin=pt;

	if(_modifiers.flags & modifiers::use_sprite_box) {

		//Uses the frame dimensions...
		bmp.set_location(_frame.box);

		if(_modifiers.flags & modifiers::match_right) {

			pt.x+=_rect.w-_frame.box.w;
		}
		else if(_modifiers.flags & modifiers::center_horizontal) {

			pt.x+=(_rect.w / 2) - (_frame.box.w / 2);
		}

		if(_modifiers.flags & modifiers::match_bottom) {

			pt.y+=_rect.h-_frame.box.h;
		}
		else if(_modifiers.flags & modifiers::center_vertical) {

			pt.y+=(_rect.h / 2) - (_frame.box.h / 2);
		}

		bmp.go_to(pt);
	}
	else {

		bmp.set_location(_rect);
	}

	bmp.set_invert_horizontal(invert_horizontal);
	bmp.set_invert_vertical(invert_vertical);

	bmp.center_rotation_center();
	bmp.set_rotation(_modifiers.rotation_degrees);

	nullptr!=camera && with_camera
		? bmp.draw(_screen, *camera)
		: bmp.draw(_screen);
}

void sprite_draw::draw(
	ldv::screen& _screen,
	ldv::rect _rect,
	const ldtools::sprite_frame& _frame
) {

	int flags=0;
	if(_frame.flags & ldtools::sprite_frame::horizontal_flip) {

		flags |= modifiers::flip_horizontal;
	}

	if(_frame.flags & ldtools::sprite_frame::vertical_flip) {

		flags |= modifiers::flip_vertical;
	}

	draw(
		_screen, _rect, _frame, 
		{
			flags,
			_frame.get_rotation()
		}
	);
}

void sprite_draw::draw(
	ldv::screen& _screen, 
	ldv::rect _rect, 
	int _index,
	sprite_draw::modifiers _flags
) {

	const auto& frame=sprite_table->get(_index);
	draw(_screen, _rect, frame, _flags);
}

void sprite_draw::draw(
	ldv::screen& _screen, 
	ldv::rect _rect, 
	int _index
) {

	const auto& frame=sprite_table->get(_index);
	draw(_screen, _rect, frame);
}

sprite_draw& sprite_draw::set_with_camera(
	bool _value
) {

	with_camera=_value;
	return *this;
}

sprite_draw& sprite_draw::set_camera(
	ldv::camera& _camera
) {

	camera=&_camera;
	return *this;
}

sprite_draw& sprite_draw::unset_camera() {

	camera=nullptr;
	return *this;
}

