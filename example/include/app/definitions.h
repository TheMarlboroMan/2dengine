#pragma once

namespace app {

static const int tile_w=16;
static const int tile_h=16;
static const int logic_screen_w=288;//at 16 units per tile this is 18 tiles per screen.
static const int logic_screen_h=224; //and these are 14 per screen.
static const int player_w=15;
static const int player_h=24;

enum spritesheets {

	ss_tiles=1
};

enum textures{
	tex_tiles=1
};

enum animation_groups{
	animgr_tiles=1
};

enum animations{
	anim_climb=103,
	anim_jump=102,
	anim_walk=101,
	anim_idle=100
};

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

enum class faces {
	left,
	right
};

}
