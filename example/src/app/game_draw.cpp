#include "app/definitions.h"
#include "app/game_draw.h"
#include "app/map.h"
#include "app/player.h"
#include "app/ladder.h"
#include "app/collectible.h"
#include "app/linear_monster.h"
#include "app/leaping_monster.h"
#include "app/timed_trap.h"
#include "app/secret_cover.h"
#include "app/button.h"
#include "app/gate.h"
#include "app/projectile.h"
#include "app/breaking_platform.h"
#include "app/platform_block.h"
#include "app/facing_block.h"
#include "app/toggle_block.h"
#include "app/exit.h"

#include <ldv/color.h>
#include <ldv/box_representation.h>
#include <ldv/resource_manager.h>
#include <d2d/video/tools.h>
#include <d2d/video/sprite_draw.h>
#include <d2d/video/sprite_fill_draw.h>

#include <tools/json.h>
#include <tools/file_utils.h>

#include <iostream>
#include <sstream>

using namespace app;

game_draw::game_draw(
	ldv::camera& _camera,
	d2d::video::scenery_tile_draw&  _scenery_tile_draw,
	d2d::video::sprite_draw&        _sprite_draw,
	d2d::video::sprite_fill_draw&        _sprite_fill_draw,
	d2d::video::animation_sprite_finder& _animation_sprite_finder,
	ldtools::ttf_manager& _ttf_manager,
	const ldv::resource_manager& _video_resource_manager,
	const appenv::env& _env
):
	camera(_camera),
	scenery_tile_draw(_scenery_tile_draw),
	sprite_draw(_sprite_draw),
	sprite_fill_draw(_sprite_fill_draw),
	animation_sprite_finder(_animation_sprite_finder),
	exit_number_font{_ttf_manager.get("exit_number_font", 8)}
{
	const std::string layout_path=_env.build_app_path("resources/layout/views.json");
	auto document=tools::parse_json_string(tools::dump_file(layout_path));

	//setup area name view
	area_name_view.map_font(
		"font",
		_ttf_manager.get("lives_banner_font", 8)
	);
	area_name_view.map_texture("main", _video_resource_manager.get_texture(app::tex_tiles));
	area_name_view.parse(document["area_name_banner"]);
	auto area_name_banner_background=area_name_view.get_by_id("area_name_bg");
	area_name_banner_background->align(
		camera.get_pos_box(),
		{
			ldv::representation_alignment::h::center,
			ldv::representation_alignment::v::inner_bottom
		}
	);

	//Setup lives banner view...
	lives_banner_view.map_font(
		"font",
		_ttf_manager.get("area_banner_font", 8)
	);
	lives_banner_view.map_texture("main", _video_resource_manager.get_texture(app::tex_tiles));
	lives_banner_view.parse(document["lives_banner"]);
	auto lives_banner_bg=lives_banner_view.get_by_id("background");
	auto lives_banner_icon=lives_banner_view.get_by_id("icon");
	auto lives_banner_text=lives_banner_view.get_by_id("lives");
	lives_banner_bg->align(
		camera.get_pos_box(),
		{
			ldv::representation_alignment::h::center,
			ldv::representation_alignment::v::inner_bottom
		}
	);

	lives_banner_icon->align(
		*lives_banner_bg,
		{
			ldv::representation_alignment::h::inner_left,
			ldv::representation_alignment::v::center,
			10, 0
		}
	);

	lives_banner_text->align(
		*lives_banner_icon,
		{
			ldv::representation_alignment::h::outer_right,
			ldv::representation_alignment::v::center,
			10, 0
		}
	);

	//And setup the rest.
	sprite_draw.set_camera(camera);
	sprite_draw.set_with_camera(true);

	sprite_fill_draw.set_camera(camera);
	sprite_fill_draw.set_with_camera(true);

	scenery_tile_draw.set_camera(camera);
	scenery_tile_draw.set_with_camera(true);
}

void game_draw::setup_lives_banner(
	int _lives
) {

	std::stringstream ss;
	ss<<"x "<<_lives;

	lives_banner_view.set_text("lives", ss.str());
}

void game_draw::setup_area_name_banner(
	const std::string& _area_name
) {

	std::stringstream ss;
	ss<<"-- "<<_area_name<<" --";

	area_name_view.set_text("area_name", ss.str());

	auto area_name_banner_text=area_name_view.get_by_id("area_name");
	auto area_name_banner_background=area_name_view.get_by_id("area_name_bg");
	area_name_banner_text->align(
		*area_name_banner_background,
		{
			ldv::representation_alignment::h::center,
			ldv::representation_alignment::v::center
		}
	);
}

void game_draw::draw_area_name_banner(ldv::screen& _screen) {

	area_name_view.draw(_screen);
}

void game_draw::draw_lives_banner(ldv::screen& _screen) {

	lives_banner_view.draw(_screen);
}

game_draw::~game_draw() {

	sprite_draw.unset_camera();
	sprite_draw.set_with_camera(false);

	sprite_fill_draw.unset_camera();
	sprite_fill_draw.set_with_camera(false);

	scenery_tile_draw.unset_camera();
	scenery_tile_draw.set_with_camera(false);
}

void game_draw::draw(
	ldv::screen& _screen,
	const app::map& _map,
	const app::player& _player,
	int _discovered_rooms
) {

	_screen.clear(_map.background_color);

	scenery_tile_draw.draw_animation(_screen, _map.background_tiles);

	//Middle tiles are like "complements" to the background.
	scenery_tile_draw.draw_animation(_screen, _map.middle_tiles);

	for(const auto& node : _map.buttons) {

		draw_button(_screen, node);
	}

	for(const auto& node : _map.gates) {

		draw_gate(_screen, node);
	}

	for(const auto& block : _map.platform_blocks) {

		draw_platform(_screen, block);
	}

	for(const auto& block : _map.breaking_platforms) {

		draw_breaking_platform(_screen, block);
	}

	for(const auto& ladder : _map.ladders) {

		draw_ladder(_screen, ladder);
	}

	for(const auto& collectible : _map.collectibles) {

		draw_collectible(_screen, collectible);
	}

	for(const auto& monster : _map.linear_monsters) {

		draw_linear_monster(_screen, monster);
	}

	for(const auto& subject : _map.leaping_monsters) {

		draw_leaping_monster(_screen, subject);
	}

	for(const auto& subject : _map.timed_traps) {

		draw_timed_trap(_screen, subject);
	}

	for(const auto& subject : _map.projectiles) {

		draw_projectile(_screen, subject);
	}

	for(const auto& subject : _map.moving_blocks) {

		draw_moving_block(_screen, subject);
	}

	for(const auto& subject : _map.facing_blocks) {

		draw_facing_block(_screen, subject);
	}

	for(const auto& subject : _map.toggle_blocks) {

		draw_toggle_block(_screen, subject);
	}

	draw_player(_screen, _player);
	scenery_tile_draw.draw_animation(_screen, _map.foreground_tiles);

	for(const auto& subject : _map.exits) {

		draw_exit(_screen, subject, _discovered_rooms);
	}

	for(const auto& secret_cover : _map.secret_covers) {

		if(secret_cover.is_discovered()) {

			continue;
		}

		draw_secret_cover(_screen, secret_cover);
	}
}

void game_draw::draw_button(
	ldv::screen& _screen,
	const app::button& _button
) {

	int sprite_index=app::spr_key_yellow;
	switch(_button.type) {

		case app::button::types::regular:           sprite_index=app::spr_regular_button; break;
		case app::button::types::yellow_keyhole:    sprite_index=app::spr_yellow_keyhole; break;
		case app::button::types::blue_keyhole:      sprite_index=app::spr_blue_keyhole; break;
		case app::button::types::red_keyhole:       sprite_index=app::spr_red_keyhole; break;
		case app::button::types::green_keyhole:     sprite_index=app::spr_green_keyhole; break;
	}

	if(_button.used) {

		switch(_button.type) {

			case app::button::types::regular:           sprite_index=app::spr_regular_button_used; break;
			case app::button::types::yellow_keyhole:    sprite_index=app::spr_yellow_keyhole_used; break;
			case app::button::types::blue_keyhole:      sprite_index=app::spr_blue_keyhole_used; break;
			case app::button::types::red_keyhole:       sprite_index=app::spr_red_keyhole_used; break;
			case app::button::types::green_keyhole:     sprite_index=app::spr_green_keyhole_used; break;
		}
	}

	auto origin=d2d::video::to_screen(_button.ent.get_origin());

	sprite_draw.draw(
		_screen,
		origin,
		sprite_index
	);
}

void game_draw::draw_gate(
	ldv::screen& _screen,
	const app::gate& _gate
) {

	auto origin=d2d::video::to_screen(_gate.ent.get_origin());

	int max_step=_gate.ent.get_h() / app::tile_h;
	for(int i=0; i<max_step; i++) { 

		int sprite_index=app::spr_gate;

		sprite_draw.draw(
			_screen,
			origin,
			sprite_index
		);

		origin.y+=app::tile_h;
	}
}

void game_draw::draw_ladder(
	ldv::screen& _screen,
	const app::ladder& _ladder
) {

	auto origin=d2d::video::to_screen(_ladder.get_origin());

	int sprite_index=app::spr_ladder_yellow;
	switch(_ladder.type) {
		case app::ladder::t_ladder: sprite_index=app::spr_ladder_yellow; break;
		case app::ladder::t_chain:  sprite_index=app::spr_chain; break;
		case app::ladder::t_vine:   sprite_index=app::spr_vine; break;
		case app::ladder::t_chain_secret: sprite_index=app::spr_chain_secret; break;
	}

	int max_step=_ladder.get_h() / app::tile_h;
	for(int i=0; i<max_step; i++) { 

		sprite_draw.draw(
			_screen,
			origin,
			sprite_index
		);

		origin.y+=app::tile_h;
	}
}

void game_draw::draw_collectible(
	ldv::screen& _screen,
	const app::collectible& _collectible
) {

	auto origin=d2d::video::to_screen(_collectible.ent.get_origin());

	int sprite_index=app::spr_gold_ingot;
	switch(_collectible.type) {

		case app::collectible::gold_ingot:  sprite_index=app::spr_gold_ingot; break;
		case app::collectible::gem:         sprite_index=app::spr_gem; break;
		case app::collectible::ruby:        sprite_index=app::spr_ruby; break;
		case app::collectible::diamond:     sprite_index=app::spr_diamond; break;
		case app::collectible::yellow_key:  sprite_index=app::spr_key_yellow; break;
		case app::collectible::blue_key:    sprite_index=app::spr_key_blue; break;
		case app::collectible::red_key:     sprite_index=app::spr_key_red; break;
		case app::collectible::green_key:   sprite_index=app::spr_key_green; break;
	}

	sprite_draw.draw(
		_screen,
		origin,
		sprite_index
	);
}

void game_draw::draw_linear_monster(
	ldv::screen& _screen,
	const app::linear_monster& _monster
) {

	//Al sprites are facing right by default.
	int flags=0;
	if(_monster.facing==app::faces::left) {

		flags |= d2d::video::sprite_draw::modifiers::flip_horizontal;
	}

	d2d::video::sprite_draw::modifiers mod{flags};

	int animation_index=0;
	switch(_monster.type) {
		case app::linear_monster::types::scorpion:

			animation_index=app::anim_scorpion;
		break;
		case app::linear_monster::types::snake:

			animation_index=app::anim_snake;
		break;
		case app::linear_monster::types::bat:

			animation_index=app::anim_bat;
		break;
	}

	const auto& line=animation_sprite_finder.get(animation_index);
	mod=animation_sprite_finder.modifiers(line, mod);

	sprite_draw.draw(
		_screen, 
		d2d::video::to_screen(_monster.ent.get_origin()),
		line.frame,
		mod
	);
}

void game_draw::draw_leaping_monster(
	ldv::screen& _screen,
	const app::leaping_monster& _monster
) {

	const auto& line=animation_sprite_finder.get(app::anim_piranha);
	auto mod=animation_sprite_finder.modifiers(line);

	sprite_draw.draw(
		_screen, 
		d2d::video::to_screen(_monster.ent.get_origin()),
		line.frame,
		mod
	);
}

void game_draw::draw_timed_trap(
	ldv::screen& _screen,
	const app::timed_trap& _trap
) {

	//If fire, There is always a plaque under this trap... We could fake it with a 
	//decoration object or with foreground tiles but those would be meh and
	//a nuisance in skill levels, respectively.
	
	if(_trap.get_type()==app::timed_trap::types::fire) {

		auto origin=d2d::video::to_screen(_trap.ent.get_origin());

		//The plaque is under the trap itself and must be centered on it.
		origin.y-=app::tile_h;
		origin.x-=(app::timed_trap::fire_w / 2) + 1;

		int sprite_index=app::spr_fire_trap_plaque;

		sprite_draw.draw(
			_screen,
			origin,
			sprite_index
		);
	}

	if(!_trap.is_harmful()) {

		return;
	}

	switch(_trap.get_type()) {

		case app::timed_trap::types::fire: {

			const auto& line=animation_sprite_finder.get(app::anim_timed_trap_fire);
			auto mod=animation_sprite_finder.modifiers(line);

			sprite_draw.draw(
				_screen, 
				d2d::video::to_screen(_trap.ent.get_origin()),
				line.frame,
				mod
			);
		}
		break;
		case app::timed_trap::types::spikes:

			sprite_draw.draw(
				_screen,
				d2d::video::to_screen(_trap.ent.get_origin()),
				app::spr_spike
			);
		break;
	}
}

void game_draw::draw_breaking_platform(
	ldv::screen& _screen,
	const app::breaking_platform& _block
) {

	if(_block.is_gone()) {

		return;
	}

	if(_block.is_ok()) {

		sprite_draw.draw(
			_screen,
			d2d::video::to_screen(_block.get_origin()),
			app::spr_breaking_block
		);
		return;
	}

	int animation_index=0;
	float anim_len=0.f;

	if(_block.is_breaking()) {

		anim_len=_block.get_breaking_ms() / 1000.f;
		animation_index=app::anim_breaking_platform;
	}
	else {// if block.is_returning

		anim_len=_block.get_returning_ms() / 1000.f;
		animation_index=app::anim_breaking_platform_return;
	}

	const auto& line=animation_sprite_finder.get(
		animation_index, 
		_block.get_timer(), 
		anim_len
	);

	auto mod=animation_sprite_finder.modifiers(line);

	sprite_draw.draw(
		_screen, 
		d2d::video::to_screen(_block.get_origin()),
		line.frame,
		mod
	);
}

void game_draw::draw_platform(
	ldv::screen& _screen,
	const app::platform_block& _block
) {

	int index=app::spr_platform_branch;
	switch(_block.get_type()) {

		case app::platform_block::types::branch:
			index=app::spr_platform_branch;
		break;
		case app::platform_block::types::rock:
			index=app::spr_platform_rock;
		break;
	}

	sprite_draw.draw(
		_screen,
		d2d::video::to_screen(_block.get_origin()),
		index
	);
}

void game_draw::draw_projectile(
	ldv::screen& _screen,
	const app::projectile& _projectile
) {

	switch(_projectile.get_type()) {

		case app::projectile::types::horizontal:
			return draw_projectile_linear(_screen, _projectile);
		case app::projectile::types::vertical:
			return draw_projectile_vertical(_screen, _projectile);
		case app::projectile::types::directed:
			return draw_projectile_directed(_screen, _projectile);
		case app::projectile::types::falling:
			return draw_projectile_falling(_screen, _projectile);
	}
}

void game_draw::draw_projectile_linear(
	ldv::screen& _screen,
	const app::projectile& _projectile
) {

	using modifiers=d2d::video::sprite_draw::modifiers;

	//Al sprites are facing right by default.
	int flags=modifiers::use_sprite_box;


	flags |= _projectile.ent.get_motion_vector().x < 0
		? modifiers::flip_horizontal
		: modifiers::match_right;

	modifiers mod{flags};

	if(!_projectile.is_moving()) {

		auto line=animation_sprite_finder.get(
			app::anim_projectile_end, 
			_projectile.get_timeout_value()
		);

		mod.flags |= modifiers::center_vertical;

		mod=animation_sprite_finder.modifiers(line, mod);

		sprite_draw.draw(
			_screen, 
			d2d::video::to_screen(_projectile.ent.get_box()),
			line.frame,
			mod
		);
		return;
	}

	auto line=animation_sprite_finder.get(app::anim_projectile);
	mod=animation_sprite_finder.modifiers(line, mod);

	sprite_draw.draw(
		_screen, 
		d2d::video::to_screen(_projectile.ent.get_box()),
		line.frame,
		mod
	);
}

void game_draw::draw_projectile_vertical(
	ldv::screen& _screen,
	const app::projectile& _projectile
) {

	using modifiers=d2d::video::sprite_draw::modifiers;
	int flags=modifiers::center_horizontal 
		| modifiers::use_sprite_box
		| modifiers::center_vertical;

	modifiers mod{flags};

	if(!_projectile.is_moving()) {

		auto line=animation_sprite_finder.get(
			app::anim_flames, 
			_projectile.get_timeout_value()
		);

		mod.flags |= modifiers::match_bottom | modifiers::center_horizontal;
		mod=animation_sprite_finder.modifiers(line, mod);

		sprite_draw.draw(
			_screen, 
			d2d::video::to_screen(_projectile.ent.get_origin()),
			line.frame,
			mod
		);
		return;
	}

	auto line=animation_sprite_finder.get(app::anim_projectile_falling_end);
	mod=animation_sprite_finder.modifiers(line, mod);

	sprite_draw.draw(
		_screen, 
		d2d::video::to_screen(_projectile.ent.get_origin()),
		line.frame,
		mod
	);
}

void game_draw::draw_projectile_directed(
	ldv::screen& _screen,
	const app::projectile& _projectile
) {

	using modifiers=d2d::video::sprite_draw::modifiers;
	int flags=modifiers::center_horizontal 
		| modifiers::use_sprite_box
		| modifiers::center_vertical;

	modifiers mod{flags};

	if(!_projectile.is_moving()) {

		auto line=animation_sprite_finder.get(
			app::anim_projectile_falling_end,
			_projectile.get_timeout_value()
		);

		sprite_draw.draw(
			_screen, 
			d2d::video::to_screen(_projectile.ent.get_origin()),
			line.frame,
			animation_sprite_finder.modifiers(line, mod)
		);
		return;
	}

	auto line=animation_sprite_finder.get(app::anim_projectile_round);

	sprite_draw.draw(
		_screen, 
		d2d::video::to_screen(_projectile.ent.get_origin()),
		line.frame,
		animation_sprite_finder.modifiers(line, mod)
	);
}

void game_draw::draw_projectile_falling(
	ldv::screen& _screen,
	const app::projectile& _projectile
) {

	using modifiers=d2d::video::sprite_draw::modifiers;
	int flags=modifiers::center_horizontal | modifiers::use_sprite_box;
	modifiers mod{flags};

	if(!_projectile.is_moving()) {

		auto line=animation_sprite_finder.get(
			app::anim_projectile_falling_end, 
			_projectile.get_timeout_value()
		);

		mod.flags |= modifiers::match_bottom;

		sprite_draw.draw(
			_screen, 
			d2d::video::to_screen(_projectile.ent.get_box()),
			line.frame,
			animation_sprite_finder.modifiers(line, mod)
		);
		return;
	}

	auto line=animation_sprite_finder.get(app::anim_projectile_falling);

	sprite_draw.draw(
		_screen, 
		d2d::video::to_screen(_projectile.ent.get_box()),
		line.frame,
		animation_sprite_finder.modifiers(line, mod)
	);
}

void game_draw::draw_secret_cover(
	ldv::screen& _screen,
	const app::secret_cover& _secret_cover
) {

	//These are no sprites, just black rectangles...
	ldv::box_representation box{
		d2d::video::to_screen_rect(_secret_cover.ent),
		ldv::rgba_color(0,0,0,255),
		ldv::box_representation::type::fill
	};

	if(_secret_cover.is_dissapearing()) {

		int alpha=255 - (_secret_cover.get_timer() * 100);
		box.set_alpha(alpha);
	}

	box.set_blend(ldv::representation::blends::alpha);
	box.draw(_screen, camera);
}

void game_draw::draw_moving_block(
	ldv::screen& _screen,
	const app::moving_block& _block
) {

	switch(_block.get_type()) {

		case 0:
			sprite_fill_draw.fill(
				_screen, 
				d2d::video::to_screen(_block.ent.get_box()),
				1
			);
			return;
		case 1:
			sprite_draw.draw(
				_screen, 
				d2d::video::to_screen(_block.ent.get_box()),
				16,
				{4, 0}
			);
			return;
	}
}

void game_draw::draw_facing_block(
	ldv::screen& _screen,
	const app::facing_block& _block
) {

	if(!_block.is_active()) {

		return;
	}

	auto box=d2d::video::to_screen(_block.ent.get_box());
	switch(_block.get_type()) {

		case 0:
			sprite_fill_draw.fill(_screen, box, 1);
		break;

	}

	int flags=0;
	if(!_block.is_facing_right()) {

		using modifiers=d2d::video::sprite_draw::modifiers;
		flags=modifiers::flip_horizontal;
	}

	d2d::video::sprite_draw::modifiers mod{flags};

	//There is an arrow centered on the block...
	auto arrow_pos=d2d::video::to_screen(
		{_block.ent.get_box().origin, app::tile_w, app::tile_h}
	);

	arrow_pos.center(box);

	sprite_draw.draw(
		_screen,
		arrow_pos,
		app::spr_block_arrow,
		mod
	);
}

void game_draw::draw_toggle_block(
	ldv::screen& _screen,
	const app::toggle_block& _block
) {

	if(!_block.is_active()) {

		return;
	}

	switch(_block.get_type()) {

		case 0:
			sprite_fill_draw.fill(
				_screen, 
				d2d::video::to_screen(_block.ent.get_box()),
				1
			);
			return;
	}
}

void game_draw::draw_player(
	ldv::screen& _screen,
	const app::player& _player
) {

	using modifiers=d2d::video::sprite_draw::modifiers;

	//Al sprites are facing right by default.
	int flags=0;
	if( _player.facing==app::faces::left) {

		flags |= modifiers::flip_horizontal;
	}

	d2d::video::sprite_draw::modifiers mod{flags};

	int animation_index=0;
	int frame_index=0;
	bool is_animation=false;

	switch(_player.state) {

		case app::player::states::ground:

			//TODO: Lol what a trap I am getting into... xD
			animation_index=_player.ent.get_motion_vector().x!=0.
				? app::anim_walk
				: app::anim_idle;
			is_animation=true;
		break;
		case app::player::states::air:
			animation_index=app::anim_jump;
		break;
		case app::player::states::crouch:
			animation_index=app::anim_crouch;
		break;
		case app::player::states::defeat:
			animation_index=app::anim_defeat;
			is_animation=true;
		break;
		case app::player::states::ladder:
			animation_index=app::anim_climb;
			int y_mod=(int)_player.ent.get_origin().y % 10;
			frame_index=abs(y_mod) <= 4 ? 0 : 1;
			//This is the wonkiest fix ever...
		break;
	}

	if(is_animation) {

		const auto& line=animation_sprite_finder.get(animation_index);
		mod=animation_sprite_finder.modifiers(line, mod);

		sprite_draw.draw(
			_screen, 
			d2d::video::to_screen(_player.ent.get_origin()),
			line.frame,
			mod
		);

		return;
	}


	const auto& animation=animation_sprite_finder.animation(animation_index);
	const auto& line=animation.get(frame_index);
	mod=animation_sprite_finder.modifiers(line, mod);

	sprite_draw.draw(
		_screen,
		d2d::video::to_screen(_player.ent.get_origin()),
		line.frame,
		mod
	);
}

void game_draw::draw_exit(
	ldv::screen& _screen,
	const app::exit& _exit,
	int _discovered_rooms
) {

	if(!_exit.min_rooms) {

		return;
	}

	auto color=_discovered_rooms < _exit.min_rooms
		? ldv::rgba8(190, 38, 51, 255)
		: ldv::rgba8(163, 206, 39, 255);

	ldv::ttf_representation text{
		exit_number_font, 
		color,
		std::to_string(_exit.min_rooms)
	};

	//Add a little black box around it...
	auto text_pos=text.get_text_position();
	text_pos.grow(4);

	ldv::box_representation box{
		text_pos,
		ldv::rgba_color(0,0,0,255),
		ldv::box_representation::type::fill
	};


	//And a white box around that one.
	text_pos.grow(2);
	ldv::box_representation border{
		text_pos,
		ldv::rgba_color(255,255,255,255),
		ldv::box_representation::type::fill
	};

	//Align everything and draw.

	auto exit_box=d2d::video::to_screen_rect(_exit.ent);
	//
	border.align(
		exit_box, 
		{
			ldv::representation_alignment::h::center,
			ldv::representation_alignment::v::outer_top,
			0, 1
		}
	);

	box.align(
		border,
		{
			ldv::representation_alignment::h::center,
			ldv::representation_alignment::v::center
		}
	);

	text.align(
		box,
		{
			ldv::representation_alignment::h::center,
			ldv::representation_alignment::v::center
		}
	);

	border.draw(_screen, camera);
	box.draw(_screen, camera);
	text.draw(_screen, camera);
}

