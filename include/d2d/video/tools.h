#pragma once

#include <ldv/rect.h>

namespace d2d { namespace video {

/**
 * converts a logic point to screen coordinates from x, y and h.
 * the logic point is expressed in absolute units (not tile
 * units).
 */
ldv::point to_screen_coordinates(ldv::point, unsigned);

}}
