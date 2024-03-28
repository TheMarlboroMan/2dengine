#pragma once

#include <map>

namespace d2d { namespace components {

/**
 * Timeouts count from zero to a limit. When the limit is reached we say the
 * timeout "is expired". The counter increases by a delta time whenever
 * "tic" is called. Once a timeout has expired it must be reset with "reset"
 * so it can be used again.
 */
class timeout {

	public:
/**
 * maximum, current and paused.
*/
	                timeout(float, float=-1.0, bool=false);

	bool            is_expired() const;
	bool            is_counting() const;
	bool            is_paused() const;
	float           get() const;
	float           get_max() const;
	timeout&        tic(float);
/**
 * sets the new maximum value.
 */
	timeout&        target(float);
/**
 * sets the current value.
 */
	timeout&        set(float);
/**
 * resets the current value to zero
 */
	timeout&        reset();
	timeout&        pause();
	timeout&        resume();

	private:

	float   timer{0.f},
			max{0.f};
	bool    paused{false};

};

/**
 * a container of timeouts associated with a numeric key.
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
 * returns the given timeout max
 */
	float           get_max(int) const;

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

	std::map<int, timeout>  data;
};
}}
