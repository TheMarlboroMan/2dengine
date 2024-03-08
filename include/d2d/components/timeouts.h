#pragma once

#include <map>

namespace d2d { namespace components {

/**
 * a container of timeouts. A timeout is a value associated to a numeric key
 * that counts down by a delta value whenever "tic" is called. When the value
 * has reached zero it has "expired". If the value is still greater than zero
 * it is "counting". Once a timeout has expired it must be reset with "reset"
 * so it can be used again.
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
 * to max. The fourth parameter indicates if the timeout should be created
 * paused.
 */

	timeouts&       add(int, float, float=-1.0, bool=false);


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

/**
 * checks if the timeout identified by the identifier is paused.
 */

	bool            is_paused(int) const;

/**
 * pauses the given timeout.
 */
	timeouts&       pause(int);

/**
 * resumes the given timeout.
 */
	timeouts&       resume(int);

	private:

	struct timeout {

		float   timer{0.f},
		        max{0.f};
		bool    paused{false};

		void    tic(float _delta);
		void    reset();
		void    pause();
		void    resume();
	};

	std::map<int, timeout>  data;
};
}}
