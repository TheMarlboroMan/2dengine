#pragma once

#include <set>

namespace app {

/**
*Simple structure to be shared with things that can tic and make noises in
*response to that. All that it does is raise some flags that the controller can
*later interpret.
*/
class tic_sound_manager {

	public:

	void    reset();
	void    add(int);

	const std::set<int>& get_indexes() const {return sound_indexes;}

	private:

	std::set<int> sound_indexes;

};
}
