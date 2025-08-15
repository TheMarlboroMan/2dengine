#pragma once

#include <cstddef> //std::size_t.

namespace app {

static const int tile_w=16;
static const int tile_h=16;
static const int logic_screen_w=288;//at 16 units per tile this is 18 tiles per screen.
static const int logic_screen_h=224; //and these are 14 per screen.
static const std::size_t particle_manager_size=100;

enum music {
	music_start=1,
	music_out,
	music_dungeon
};

enum sound {
	snd_item_pickup=1,
	snd_jump=2,
	snd_open_gate=3,
	snd_projectile=4,
	snd_switch=5,
	snd_defeat=6,
	snd_secret=7,
	snd_fire=8,
	snd_launch_player=9,
	snd_block_break=10,
	snd_forbidden=11
};

/**
 * these will behave as flags, DooM style.
 **/
enum skill_settings {
	skill_easy=1,
	skill_normal=2,
	skill_hard=4
};

enum persistence_groups {
	pergr_collectibles=1, //regular collectible bonuses
	pergr_secret_covers=2, //black rectangles covering secret areas.
	pergr_buttons=3, //buttons and keyholes.
	pergr_touch_triggers=4, //touch triggers
	pergr_automap=5, //automap discovery state.
};

enum automap_flags {
	am_discovered=1, //room has been visited
	am_complete=2 //room has no collectibles left
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
	//animated tiles
	anim_water_surface=1,
	anim_waterfall=2,
	anim_lava_surface=3,
	anim_wall_torch=4,

	//player
	anim_idle=110,
	anim_walk=111,
	anim_jump=112,
	anim_climb=113,
	anim_crouch=114,
	anim_defeat=115,

	//monsters and traps
	anim_scorpion=150,
	anim_projectile=151,
	anim_projectile_end=152, //TODO: I think this is not used anymore.
	anim_snake=153,
	anim_bat=154,
	anim_piranha=155,
	anim_projectile_round=157,
	anim_timed_trap_fire=159,
	anim_projectile_falling=203,
	anim_projectile_vertical_flame=205,

	//particles...
	anim_flames=156,
	anim_projectile_round_end=158,
	anim_projectile_falling_end=204,
	anim_smoke=206,

	//other stuff
	anim_breaking_platform=200,
	anim_breaking_platform_return=201,
};

enum sprites { 
	spr_water_surface=500, 
	spr_waterfall=504,
	spr_lava_surface=508,
	spr_wall_torch=215,

	//Collectibles
	spr_gold_ingot=300,
	spr_gem=301,
	spr_ruby=302,
	spr_key_yellow=303,
	spr_key_blue=304,
	spr_key_red=305,
	spr_key_green=306,
	spr_diamond=307,

	//Special world elements
	spr_spike=169,
	spr_ladder_yellow=350,
	spr_gate=351,
	spr_chain=368,
	spr_chain_secret=352,
	spr_vine=353,
	spr_yellow_keyhole=354,
	spr_blue_keyhole=355,
	spr_red_keyhole=356,
	spr_green_keyhole=366,
	spr_yellow_keyhole_used=357,
	spr_blue_keyhole_used=358,
	spr_red_keyhole_used=359,
	spr_green_keyhole_used=367,
	spr_regular_button=360,
	spr_regular_button_used=361,
	spr_breaking_block=362,
	spr_platform_branch=155,
	spr_fire_trap_plaque=188,
	spr_platform_rock=189,
	spr_block_arrow=512,
	spr_particle_breaking_plat_1=514,
	spr_particle_breaking_plat_2=515,
	spr_particle_breaking_plat_3=516,
	spr_particle_breaking_plat_4=517,
	spr_particle_bonus_1=518,
	spr_particle_bonus_2=519,
	spr_particle_bonus_3=520,
	spr_particle_bonus_4=521,
	spr_particle_smoke_1=522,
	spr_particle_smoke_2=523,
	spr_particle_smoke_3=524,
	spr_particle_smoke_4=525,

	spr_boss_center=420,
	spr_boss_side=421,
	spr_boss_skull=422
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
	tile_full_no_camera=14, //fully solid block, does not restrict the camera
	tile_camera_stop=15, //non solid block, camera affecting.
	tile_harm_half_bottom=16,
	tile_harm_half_top=17,
	tile_harm_half_left=18,
	tile_harm_half_right=19
};

enum class faces {
	left, //assume 0
	right //assume 1
};

enum particles {
	prt_flame=0, //flame that appears before the boss fires vertical barrages.
	prt_projectile_splash=1,
	prt_projectile_horizontal_splash=2,
	prt_breaking_platform=3,
	prt_bonus=4,
	prt_smoke=5
};

}
