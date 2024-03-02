#pragma once

#include <map>

namespace d2d { namespace components {
/**
 * a container of timeouts.
 */
class timeouts {

	public:

/**
 * tics all timeouts.
 */
	void            tic(float);

/**
 * returns the given timeout value
 */
	float           get(int) const;

/**
 * adds a new timeout to the list with an identifier and its max value. If
 * the third parameter is not given the value of the timeout will be set
 * to max.
 */

	timeouts&       add(int, float, float=-1.0);


/**
 * forces the timeout to be set at the given value.
 */ 
	timeouts&       set(int, float);

/**
 * resets the timeout identified by the identifier to its max value.
 */

	timeouts&       reset(int);

/**
 * checks if the timeout identified by the identifier is ready (that is, 
 * its counter has reached zero.
 */
	bool            is_expired(int) const;

/**
 * checks if the timeout identified by the identifier is still counting down.
 */
	bool            is_counting(int) const;

	private:

	struct timeout {

		float   timer{0.f},
		        max{0.f};

		void    tic(float _delta) {

			timer-=_delta;
			if(timer < 0.f) {

				timer=0.f;
			}
		}

		void    reset() {

			timer=max;
		}
	};

	std::map<int, timeout>  data;
};
}}
