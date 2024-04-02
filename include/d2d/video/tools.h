#pragma once

#include "../collision/definitions.h"
#include <ldv/rect.h>

namespace d2d { namespace collision {

struct spatiable;
}}

namespace d2d { namespace video {

/**
 * converts a collision point to a screen point. This is done by casting
 * the components to integers, rounded down. No inversion of the y component
 * is done so this function is suitable for centering cameras on spatiable
 * elements.
 */

//TODO: are there terrible names??? Should be "to_cartesian_screen"? To_cartesian_camera?
ldv::point to_screen(d2d::collision::point);
ldv::rect to_screen(const d2d::collision::box&);
ldv::rect to_screen(const d2d::collision::spatiable&);

/**
 * converts a logic point to screen coordinates from x, y and h.
 * the logic point is expressed in absolute units (not tile
 * units). The y component is inverted.
 */
ldv::point to_screen_coordinates(ldv::point, unsigned);

/**
 * converts collision boxes to screen coordinates by manipulating the y
 * component.
 */
ldv::rect to_screen_rect(const d2d::collision::box&);
ldv::rect to_screen_rect(const d2d::collision::spatiable&);

}}
