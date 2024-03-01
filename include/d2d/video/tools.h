#pragma once

#include "../collision/definitions.h"
#include <ldv/rect.h>

namespace d2d { namespace collision {

struct spatiable;
}}

namespace d2d { namespace video {

/**
 * converts a collision point to a screen point. This is done by casting
 * the components to integers, rounded down.
 */

ldv::point to_screen(d2d::collision::point);

/**
 * converts a logic point to screen coordinates from x, y and h.
 * the logic point is expressed in absolute units (not tile
 * units).
 */
ldv::point to_screen_coordinates(ldv::point, unsigned);

/**
 * converts collision boxes to screen coordinates.
 */
ldv::rect to_screen_rect(const d2d::collision::box&);
ldv::rect to_screen_rect(const d2d::collision::spatiable&);

}}
