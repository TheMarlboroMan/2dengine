#include "d2d/video/sprite_draw.h"
#include "d2d/video/exception.h"
#include "d2d/video/tools.h"
#include <iostream>

using namespace d2d::video;

sprite_draw::sprite_draw(
	const ldtools::sprite_table& _sprite_table,
	const ldv::texture& _texture
): 
	bmp{_texture},
	sprite_table{&_sprite_table}
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

void sprite_draw::draw(
	ldv::screen& _screen,
	const ldv::camera& _camera,
	ldv::point _point,
	const ldtools::sprite_frame& _frame
) {

	bmp.set_clip({{_frame.box.origin.x, _frame.box.origin.y}, _frame.box.w, _frame.box.h});
	auto pt=d2d::video::to_screen_coordinates(_point, _frame.box.h);
	bmp.set_location({pt, _frame.box.w, _frame.box.h});
	//TODO: What about the thing being flipped and stuff???
	bmp.draw(_screen, _camera);
}

void sprite_draw::draw(
	ldv::screen& _screen,
	ldv::point _point,
	const ldtools::sprite_frame& _frame
) {

	bmp.set_clip({{_frame.box.origin.x, _frame.box.origin.y}, _frame.box.w, _frame.box.h});
	auto pt=d2d::video::to_screen_coordinates(_point, _frame.box.h);
	bmp.set_location({pt, _frame.box.w, _frame.box.h});
	//TODO: What about the thing being flipped and stuff???
	bmp.draw(_screen);
}

void sprite_draw::draw(
	ldv::screen& _screen, 
	const ldv::camera& _camera, 
	ldv::point _point, 
	int _index
) {

	//TODO: What about the thing being flipped and stuff???
	const auto& frame=sprite_table->get(_index);
	draw(_screen, _camera, _point, frame);
}

void sprite_draw::draw(
	ldv::screen& _screen, 
	ldv::point _point, 
	int _index
) {

	const auto& frame=sprite_table->get(_index);

	//TODO: sure, what about flipping shit my man???

	draw(_screen, _point, frame);
}
