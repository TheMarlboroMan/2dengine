#pragma once

#include "definitions.h"
#include "spatiable.h"

namespace d2d { namespace collision {

//These are always to be read as is_(first_arg)_verb_of(second_arg).

bool           collides_with(const spatiable&, const spatiable&);
bool           collides_with(const spatiable&, const box&);
bool           collides_with(const box&, const box&);
bool           is_above(const spatiable&, const spatiable&);
bool           is_above(const spatiable&, const box&);
bool           is_above(const box&, const box&);
bool           is_below(const spatiable&, const spatiable&);
bool           is_below(const spatiable&, const box&);
bool           is_below(const box&, const box&);
bool           is_left_of(const spatiable&, const spatiable&);
bool           is_left_of(const spatiable&, const box&);
bool           is_left_of(const box&, const box&);
bool           is_right_of(const spatiable&, const spatiable&);
bool           is_right_of(const spatiable&, const box&);
bool           is_right_of(const box&, const box&);

//These should be read as do_funcname to first arg using second arg as reference.
void           snap_to_right_of(spatiable&, const spatiable&);
void           snap_to_left_of(spatiable&, const spatiable&);
void           snap_to_top_of(spatiable&, const spatiable&);
void           snap_to_bottom_of(spatiable&, const spatiable&);
void           match_right_of(spatiable&, const spatiable&);
void           match_left_of(spatiable&, const spatiable&);
void           match_top_of(spatiable&, const spatiable&);
void           match_bottom_of(spatiable&, const spatiable&);
void           center_horizontally(spatiable&, const spatiable&);
void           center_horizontally(box&, const box&);
void           center_vertically(spatiable&, const spatiable&);
void           center_vertically(box&, const box&);

}}
