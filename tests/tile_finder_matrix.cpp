#include <iostream>
#include <vector>
#include <d2d/collision/tile.h>
#include <d2d/storage/map_loader.h>
#include <app/tile_impl.h>
#include <d2d/collision/tile_finder_matrix.h>
#include <d2d/collision/shaper_default.h>

int main(
	int _argc,
	char ** _argv
) {

	if(2 != _argc) {

		std::cout<<"usage: tile_finder_matrix path_to_map_file"<<std::endl;
		return 1;
	}

	d2d::storage::map_loader ml(_argv[1]);

	std::vector<d2d::collision::tile> tiles;
	app::tile_impl tile_impl;
	d2d::collision::shaper_default shaper_default(24, 24);

	ml.load_collision_tiles(
		"logic",
		tiles,
		shaper_default,
		tile_impl
	);

	std::cout<<"loaded "<<tiles.size()<<" tiles"<<std::endl;

	d2d::collision::tile_finder_matrix tfm(24, 24);
	tfm.load(tiles);

	std::cout<<"tile finder loaded"<<std::endl;

	while(true) {

		std::cout<<"enter x y. ctrl-c to exit\n>> ";
		int x, y;
		std::cin>>x>>y;

		if(!tfm.has({x,y})) {

			std::cout<<"nothing there"<<std::endl;
			continue;
		}

		auto ptr=tfm.get({x,y});
		std::cout<<"type at "<<x<<","<<y<<" is "<<ptr->type<<std::endl;
	}

	return 0;
}
