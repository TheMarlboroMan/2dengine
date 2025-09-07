#pragma once

#include <d2d/collision/spatiable_box.h>
#include <d2d/motion/definitions.h>
#include <iostream>

namespace app {

//The spatiable_box used to be an "entity" in this application before it was
//promoted to the library.
using entity=d2d::collision::spatiable_box;

}
