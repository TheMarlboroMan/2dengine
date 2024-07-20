#pragma once
namespace app {

/**
 * Things that can be held by the player in all skill levels.
 */
struct inventory {

	int         yellow_keys{0},
	            blue_keys{0},
	            red_keys{0},
	            green_keys{0},
				//TODO: Does this actually make any sense??
	            total_collectibles{0};

	void        reset();
};

}
