#include "d2d/video/scenery_tile_draw.h"
#include "d2d/video/exception.h"
#include <iostream>

using namespace d2d::video;

scenery_tile_draw::scenery_tile_draw(
	int _tile_w,
	int _tile_h,
	const ldtools::sprite_table& _sprite_table,
	const ldv::texture& _texture
):
	tile_w{_tile_w},
	tile_h{_tile_h},
	bmp{_texture, {0,0, (unsigned)_tile_w, (unsigned)_tile_h}, {0,0,(unsigned)_tile_w, (unsigned)_tile_h}},
	sprite_table{&_sprite_table}
{

	bmp.set_blend(ldv::representation::blends::alpha);
}

scenery_tile_draw& scenery_tile_draw::reset_sprite_table(
	const ldtools::sprite_table& _table
) {

	sprite_table=&_table;
	return *this;
}

scenery_tile_draw& scenery_tile_draw::reset_texture(
	const ldv::texture& _texture
) {

	bmp.set_texture(_texture);
	return *this;
}

void scenery_tile_draw::draw(
	ldv::screen& _screen,
	const ldv::camera& _camera,
	const scenery_tile& _tile
) {

	try {

		const auto& frame=sprite_table->get(_tile.type);

		bmp.set_clip({{frame.box.origin.x, frame.box.origin.y}, frame.box.w, frame.box.h});

		int x=tile_w*_tile.x;
		int y=tile_h*_tile.y;

		//These must be now positioned in SCREEN coordinates...
		y=-1*y-frame.box.h;

		bmp.go_to({x, y});

		bmp.draw(_screen, _camera);
	}
	catch(ldtools::sprite_table_exception& e) {

		throw exception(e.what());
	}
}

void scenery_tile_draw::draw(
	ldv::screen& _screen,
	const scenery_tile& _tile
) {

	try {

		const auto& frame=sprite_table->get(_tile.type);

		bmp.set_clip({{frame.box.origin.x, frame.box.origin.y}, frame.box.w, frame.box.h});

		int x=tile_w*_tile.x;
		int y=tile_h*_tile.y;

		y=-1*y-frame.box.h;
		bmp.go_to({x, y});

		bmp.draw(_screen);
	}
	catch(ldtools::sprite_table_exception& e) {

		throw exception(e.what());
	}
}
