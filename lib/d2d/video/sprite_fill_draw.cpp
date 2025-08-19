#include "d2d/video/sprite_fill_draw.h"
//#include "d2d/video/exception.h"
#include "d2d/video/tools.h"
#include <iostream>

using namespace d2d::video;

sprite_fill_draw::sprite_fill_draw(
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

sprite_fill_draw& sprite_fill_draw::reset_sprite_table(
	const ldtools::sprite_table& _table
) {

	sprite_table=&_table;
	return *this;
}

sprite_fill_draw& sprite_fill_draw::reset_texture(
	const ldv::texture& _texture
) {

	bmp.set_texture(_texture);
	return *this;
}

sprite_fill_draw& sprite_fill_draw::set_with_camera(
	bool _value
) {

	with_camera=_value;
	return *this;
}

sprite_fill_draw& sprite_fill_draw::set_camera(
	ldv::camera& _camera
) {

	camera=&_camera;
	return *this;
}

sprite_fill_draw& sprite_fill_draw::unset_camera() {

	camera=nullptr;
	return *this;
}

void sprite_fill_draw::fill(
	ldv::screen& _screen,
	ldv::rect _rect,
	int _index
) {

	const auto& frame=sprite_table->get(_index);
	modifiers mod;
	fill(_screen, _rect, frame, mod);
}

void sprite_fill_draw::fill(
	ldv::screen& _screen,
	ldv::rect _rect,
	int _index,
	modifiers _modifiers
) {

	const auto& frame=sprite_table->get(_index);
	fill(_screen, _rect, frame, _modifiers);
}


void sprite_fill_draw::fill(
	ldv::screen& _screen,
	ldv::rect _rect,
	const ldtools::sprite_frame& _frame
) {

	modifiers mod;
	fill(_screen, _rect, _frame, mod);
}

void sprite_fill_draw::fill(
	ldv::screen& _screen,
	ldv::rect _rect,
	const ldtools::sprite_frame& _frame,
	modifiers _modifiers
) {

	//This is the one method the rest end up in.
	auto pt=d2d::video::to_screen_coordinates(_rect.origin, _rect.h);

	bmp.set_clip({{_frame.box.origin.x, _frame.box.origin.y}, _frame.box.w, _frame.box.h});
	bmp.set_brush(_frame.box.w, _frame.box.h);
	bmp.set_location(_rect);
	bmp.go_to(pt);

	bool invert_horizontal=_modifiers.flags & modifiers::flip_horizontal;
	bool invert_vertical=_modifiers.flags & modifiers::flip_vertical;

	bmp.set_invert_horizontal(invert_horizontal);
	bmp.set_invert_vertical(invert_vertical);

	bmp.center_rotation_center();
	bmp.set_rotation(_modifiers.rotation_degrees);

	bmp.set_alpha(_modifiers.alpha);

	nullptr!=camera && with_camera
		? bmp.draw(_screen, *camera)
		: bmp.draw(_screen);
}

