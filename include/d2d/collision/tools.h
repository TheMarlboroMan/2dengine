#pragma once

#include "definitions.h"
#include "spatiable.h"

//TODO:
#include <iostream>
#include <iomanip>
#include <cstdio>

namespace d2d { namespace collision {

//TODO:
template<typename T, typename U> 
bool shit_overlap(T x1, T y1, U w1, U h1, T x2, T y2, U w2, U h2) {

	T 	endx2=x2+(T)w2,
		endx1=x1+(T)w1, //This is the one: 244.00000000003 + 12 = 256.0
		endy2=y2+(T)h2,
		endy1=y1+(T)h1;

/**
 * My options:
 *  - fucking magic
 *  - assume that touch IS collision. Damned be all... This will likely break
 *      most shit that is already there.in the library, right? For the broad
 *      
 */


	//constexpr T EPS=(T)1e-9;
	bool x_second_end_before_first_begin=endx2 <= x1;// + EPS;
	bool x_second_begin_after_first_end=x2 >= endx1;// - EPS;
	bool y_second_end_before_first_begin=endy2 <= y1;// + EPS;
	bool y_second_begin_after_first_end=y2 >= endy1;// - EPS;
	bool in_x=!(x_second_end_before_first_begin || x_second_begin_after_first_end);
	bool in_y=!(y_second_end_before_first_begin || y_second_begin_after_first_end);

/**
std::cout<<"===\n";
std::cout<<"x_second_end_before_first_begin="<<x_second_end_before_first_begin<<"\n";
std::cout<<"x_second_begin_after_first_end="<<x_second_begin_after_first_end<<"\n";
std::cout<<"y_second_end_before_first_begin="<<y_second_end_before_first_begin<<"\n";
std::cout<<"y_second_begin_after_first_end="<<y_second_begin_after_first_end<<"\n";
std::cout<<std::fixed<<std::setprecision(20);
std::cout<<"in x="<<in_x<<" in y="<<in_y<<"\n";
std::cout<<"1) "<<x1<<","<<y1<<" "<<w1<<"x"<<h1<<" endx="<<endx1<<" endy="<<endy1<<"\n";
std::cout<<"2) "<<x2<<","<<y2<<" "<<w2<<"x"<<h2<<" endx="<<endx2<<" endy="<<endy2<<"\n";

*/
	if(in_x && in_y) {

		return true;
	}

	return false;
}



/**
*Suite of collision and resolving primitives for aabb vs aabb boxes.
*/

//These are always to be read as is_(first_arg)_verb_of(second_arg).
bool            collides_with(const spatiable&, const spatiable&, bool=false);
bool            collides_with(const spatiable&, const box&, bool=false);
bool            collides_with(const box&, const box&, bool=false);
bool            collides_with(const box&, const spatiable&, bool=false);
//!Is above means the lower edge is above the upper edge.
bool            is_above(const spatiable&, const spatiable&);
bool            is_above(const spatiable&, const box&);
bool            is_above(const box&, const box&);
bool            is_above(const box&, const spatiable&);
//!Is below means the upper edge is below the loger edge.
bool            is_below(const spatiable&, const spatiable&);
bool            is_below(const spatiable&, const box&);
bool            is_below(const box&, const box&);
bool            is_below(const box&, const spatiable&);
//!Is left of means the right edge is left of the left edge.
bool            is_left_of(const spatiable&, const spatiable&);
bool            is_left_of(const spatiable&, const box&);
bool            is_left_of(const box&, const box&);
bool            is_left_of(const box&, const spatiable&);
//!Is right of means the left edge is right of the right edge.
bool            is_right_of(const spatiable&, const spatiable&);
bool            is_right_of(const spatiable&, const box&);
bool            is_right_of(const box&, const spatiable&);
bool            is_right_of(const box&, const box&);

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

//These should be read as do_funcname to first arg using second arg as reference.
//When a double is present it is meant to be a "padding", so if we say 
//"match top of this with 10 of padding" it will match the top and then move
//10 units down. Marging should always be expressed in positive to move "away"
//from the matched spatiable.
void           snap_to_right_of(spatiable&, const spatiable&);
void           snap_to_left_of(spatiable&, const spatiable&);
void           snap_to_top_of(spatiable&, const spatiable&);
void           snap_to_bottom_of(spatiable&, const spatiable&);
void           match_right_of(spatiable&, const spatiable&, double=0.);
void           match_left_of(spatiable&, const spatiable&, double=0.);
void           match_top_of(spatiable&, const spatiable&, double=0.);
void           match_bottom_of(spatiable&, const spatiable&, double=0.);
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
