#include "app/tic_sound_manager.h"

using namespace app;

void tic_sound_manager::reset() {

	sound_indexes.clear();
}

void tic_sound_manager::add(
	int _index
) {

	sound_indexes.insert(_index);
}

