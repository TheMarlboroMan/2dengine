#pragma once

#include "definitions.h"

namespace d2d { namespace motion {

/**
*A bit of a stupid class, but here it is... Can take a vector or a 
*portion of a vector and accelerate / decelerate it. I guess I can put some
*clamping on it, to make it worth it.
*/
class accelerator {

	public:

	//The constructor specifies the clamping. Zero values mean "no clamping".
	                    accelerator(const motion_vector&);

	//Resets the clamp. Zero values mean "no clamp".
	void                clamp(const motion_vector&);

	//Accelerates the vector away from zero by the given factor. Makes negative
	//values "lesser" and positive values "larger". For zero values, the 
	//value passed is decided to know if we move towards negative or positive
	//values. Clamping is used as the max value.
	void                accelerate_x(motion_vector&, t_motion);
	void                accelerate_y(motion_vector&, t_motion);
	void                accelerate(motion_vector&, t_motion);
	void                accelerate(motion_vector&, const motion_vector&);

	//Makes the vector closer to zero. Will not reach the "other" side from
	//positive to negative (or vice versa). Clamping is used as the min
	//value. The sign of the value passed can be discarded.
	void                decelerate_x(motion_vector&, t_motion);
	void                decelerate_y(motion_vector&, t_motion);
	void                decelerate(motion_vector&, t_motion);
	void                decelerate(motion_vector&, const motion_vector&);

	private:

	void                away_from_zero(t_motion&, t_motion, t_motion);
	void                towards_zero(t_motion&, t_motion, t_motion);

	motion_vector       clamp_vector;
};
}}
