#pragma once

namespace app {

static const int tile_w=16;
static const int tile_h=16;
static const int logic_screen_w=288;//at 16 units per tile this is 18 tiles per screen.
static const int logic_screen_h=224; //and these are 14 per screen.

enum persistence_groups {
	pergr_collectibles=1, //regular collectible bonuses
	pergr_secret_covers=2, //black rectangles covering secret areas.
	pergr_buttons=3 //buttons and keyholes.
};

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
	anim_water_surface=1,
	anim_waterfall=2,
	anim_lava_surface=3,
	anim_m_idle=100,
	anim_m_walk=101,
	anim_m_jump=102,
	anim_m_climb=103,
	anim_m_crouch=104,
	anim_m_defeat=105,
	anim_f_idle=110,
	anim_f_walk=111,
	anim_f_jump=112,
	anim_f_climb=113,
	anim_f_crouch=114,
	anim_f_defeat=115,
	anim_scorpion=150
};

enum sprites {
	spr_water_surface=500,
	spr_waterfall=504,
	spr_lava_surface=508,

	//Collectibles
	spr_gold_ingot=300,
	spr_gem=301,
	spr_ruby=302,
	spr_key_yellow=303,
	spr_key_blue=304,
	spr_key_red=305,

	//Special world elements
	spr_ladder_yellow=350,
	spr_gate=351,
	spr_chain=352,
	spr_vine=353,
	spr_yellow_keyhole=354,
	spr_blue_keyhole=355,
	spr_red_keyhole=356,
	spr_regular_button=131
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
	tile_harm=12,
	tile_monster_block=13,
};

enum class faces {
	left,
	right
};

}
