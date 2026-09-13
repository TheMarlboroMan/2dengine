#pragma once

#include "definitions.h"
#include "spatiable.h"

namespace d2d { namespace collision {

/**
*Suite of collision and resolving primitives for aabb vs aabb boxes.
*/

//These are always to be read as is_(first_arg)_verb_of(second_arg).
//Touching edges are not collisions. Any overlap must be larger than an 
//epsilon value to be considered a collision.
bool            collides_with(const spatiable&, const spatiable&, t_position=default_epsilon);
bool            collides_with(const spatiable&, const box&, t_position=default_epsilon);
bool            collides_with(const box&, const box&, t_position=default_epsilon);
bool            collides_with(const box&, const spatiable&, t_position=default_epsilon);
//!Is above means the lower edge is above or at the same position of the upper edge.
//!An overlap smaller than epsilon is neglible and treated as lhs is above rhs.
bool            is_above(const spatiable&, const spatiable&, t_position=default_epsilon);
bool            is_above(const spatiable&, const box&, t_position=default_epsilon);
bool            is_above(const box&, const box&, t_position=default_epsilon);
bool            is_above(const box&, const spatiable&, t_position=default_epsilon);
//!Is below means the upper edge is below or equal to the loger edge. 
//!An overlap smaller than epsilon is neglible and treated as lhs is below rhs.
bool            is_below(const spatiable&, const spatiable&, t_position=default_epsilon);
bool            is_below(const spatiable&, const box&, t_position=default_epsilon);
bool            is_below(const box&, const box&, t_position=default_epsilon);
bool            is_below(const box&, const spatiable&, t_position=default_epsilon);
//!Is left of means the right edge is left of or equal to the left edge.
//!An overlap smaller than epsilon is neglible and treated as lhs is left of rhs.
bool            is_left_of(const spatiable&, const spatiable&, t_position=default_epsilon);
bool            is_left_of(const spatiable&, const box&, t_position=default_epsilon);
bool            is_left_of(const box&, const box&, t_position=default_epsilon);
bool            is_left_of(const box&, const spatiable&, t_position=default_epsilon);
//!Is right of means the left edge is right of or equal to the right edge.
//!An overlap smaller than epsilon is neglible and treated as lhs is right of rhs.
bool            is_right_of(const spatiable&, const spatiable&, t_position=default_epsilon);
bool            is_right_of(const spatiable&, const box&, t_position=default_epsilon);
bool            is_right_of(const box&, const spatiable&, t_position=default_epsilon);
bool            is_right_of(const box&, const box&, t_position=default_epsilon);

//!Is above means the lower edge is at the same position of the upper edge.
//!An overlap smaller than epsilon is neglible and treated as lhs is just above rhs.
bool            is_just_above(const spatiable&, const spatiable&, t_position=default_epsilon);
bool            is_just_above(const spatiable&, const box&, t_position=default_epsilon);
bool            is_just_above(const box&, const box&, t_position=default_epsilon);
bool            is_just_above(const box&, const spatiable&, t_position=default_epsilon);

//!Is partially above means the top edge is above the upper edge, but the
//!bottom edge is not.
bool            is_partially_above(const spatiable&, const spatiable&);
bool            is_partially_above(const spatiable&, const box&);
bool            is_partially_above(const box&, const spatiable&);
bool            is_partially_above(const box&, const box&);
//!Is partially below means the lower edge is below the loger edge but the
//!upper one is not.
bool            is_partially_below(const spatiable&, const spatiable&);
bool            is_partially_below(const spatiable&, const box&);
bool            is_partially_below(const box&, const spatiable&);
bool            is_partially_below(const box&, const box&);
//!Is partially left of means the left edge is left of the left edge but the
//!right one is not.
bool            is_partially_left_of(const spatiable&, const spatiable&);
bool            is_partially_left_of(const spatiable&, const box&);
bool            is_partially_left_of(const box&, const spatiable&);
bool            is_partially_left_of(const box&, const box&);
//!Is partially right of means the right edge is right of the right edge but
//!the left one is not.
bool            is_partially_right_of(const spatiable&, const spatiable&);
bool            is_partially_right_of(const spatiable&, const box&);
bool            is_partially_right_of(const box&, const spatiable&);
bool            is_partially_right_of(const box&, const box&);
//!Is edge above means the bottom edge of A is above the bottom edge of B.
bool            is_edge_above(const spatiable&, const spatiable&);
bool            is_edge_above(const spatiable&, const box&);
bool            is_edge_above(const box&, const spatiable&);
bool            is_edge_above(const box&, const box&);
//!Is edge below means the bottom edge of A is below the bottom edge of B.
bool            is_edge_below(const spatiable&, const spatiable&);
bool            is_edge_below(const spatiable&, const box&);
bool            is_edge_below(const box&, const spatiable&);
bool            is_edge_below(const box&, const box&);
//!Is edge left of means the left edge of A is left of the left edge of B
bool            is_edge_left_of(const spatiable&, const spatiable&);
bool            is_edge_left_of(const spatiable&, const box&);
bool            is_edge_left_of(const box&, const spatiable&);
bool            is_edge_left_of(const box&, const box&);
//!Is edge right of means the left edge of A is right of the left edge of B
bool            is_edge_right_of(const spatiable&, const spatiable&);
bool            is_edge_right_of(const spatiable&, const box&);
bool            is_edge_right_of(const box&, const spatiable&);
bool            is_edge_right_of(const box&, const box&);

//These should be read as do_funcname to first arg using second arg as reference.
//When a double is present it is meant to be a "padding", so if we say 
//"match top of this with 10 of padding" it will match the top and then move
//10 units down. Margins should always be expressed in positive to move "away"
//from the matched spatiable.
void           snap_to_right_of(spatiable&, const spatiable&);
void           snap_to_left_of(spatiable&, const spatiable&);
void           snap_to_top_of(spatiable&, const spatiable&);
void           snap_to_bottom_of(spatiable&, const spatiable&);
void           match_right_of(spatiable&, const spatiable&, t_position=0.);
void           match_left_of(spatiable&, const spatiable&, t_position=0.);
void           match_top_of(spatiable&, const spatiable&, t_position=0.);
void           match_bottom_of(spatiable&, const spatiable&, t_position=0.);
void           center_horizontally(spatiable&, const spatiable&);
void           center_horizontally(spatiable&, const box&);
void           center_horizontally(box&, const box&);
void           center_vertically(spatiable&, const spatiable&);
void           center_vertically(spatiable&, const box&);
void           center_vertically(box&, const box&);

/**
 * returns true if the edge (as in the edge enum) has two opposing values
 * at the same time.
 */
bool           is_compromised_edge(int);

}}
