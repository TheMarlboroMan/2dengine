#pragma once

namespace app {

static const int tile_w=24;
static const int tile_h=24;

enum tile_types {
		tile_full=1,
		tile_half_bottom=2,
		tile_half_top=3,
		tile_half_left=4,
		tile_half_right=5,
		tile_quarter_bottom_left=6,
		tile_quarter_bottom_right=7,
		tile_quarter_top_left=8,
		tile_quarter_top_right=9,
		tile_half_bottom_passable=10,
		tile_half_top_passable=11,
		tile_harm=12
	};
}
