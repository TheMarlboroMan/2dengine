#pragma once

#include <map>
#include <ostream>
#include <ldtools/time_definitions.h>

namespace d2d { namespace components {

using ldtools::tdelta;

/**
 * Timeouts count from zero to a limit. When the limit is reached we say the
 * timeout "is is_finished". The counter increases by a delta time whenever
 * "tic" is called. Once a timeout has expired it must be reset with "reset"
 * so it can be used again. Once a timeout has expired it pauses on its own!
 */
class timeout {

	public:

//! maximum, current and paused.
	                timeout(tdelta, tdelta=0., bool=false);

//! Returns true if the timeout has reached its limit.
	bool            is_finished() const;

//! Returns true if the timeout has not reached its limit and is still running.
	bool            is_running() const;
//! Returns true if the timer is paused (finished or not).
	bool            is_paused() const;
//! Returns the current value.
	tdelta get() const;
//! Returns the max value.
	tdelta get_max() const;
//! Tics this timer for the given delta value. If a timer does not tic, time does not pass for it.
	timeout&        tic(tdelta);
//! sets the new maximum value.
	timeout&        target(tdelta);
//! sets the current value.
	timeout&        set(tdelta);
//! resets the current value to zero, does not resume!
	timeout&        reset();
//! Pauses the timeout. A paused timeout does nothing when it tics.
	timeout&        pause();
//! resumes the counter after pausing.
	timeout&        resume();
//! resets and resumes in a single step.
	timeout&        restart();

	private:

	tdelta   timer{0.}, //<! Internal timer.
	        max{0.};    //<! Maximum value.
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
	void            tic(tdelta);

/**
 * tics one timeout
 */
	void            tic(int, tdelta);

/**
 * returns the given timeout value
 */
	tdelta get(int) const;

/**
 * returns the given timeout.
 */
	timeout&        at(int _id) {return data.at(_id);}

/**
 * returns the given timeout.
 */
	const timeout&  at(int _id) const {return data.at(_id);}

/**
 * returns the given timeout max
 */
	tdelta get_max(int) const;

/**
 * adds a new timeout to the list with an identifier and its max value. If
 * the third parameter is not given the value of the timeout will be set
 * to max. The fourth parameter indicates if the timeout should be created
 * paused.
 */

	timeouts&       add(int, tdelta, tdelta=0., bool=false);


/*
 * forces the timeout to be set at the given value and returns it.
 */ 
	timeout&       set(int, tdelta);

/**
 * resets the timeout identified by the identifier to its max value and
 * returns that timeout.
 */
	timeout&       reset(int);

/**
 * changes the target for the given timeout and returns that timeout.
 */
	timeout&       target(int, tdelta);

/**
 * resets all timeouts.
 */
	timeouts&       reset();

/**
 * pauses all timeouts.
 */
	timeouts&       pause();

/**
 * restart all timeouts.
 */
	timeouts&       restart();

/**
 * checks if the timeout identified by the identifier is ready (that is, 
 * its counter has reached zero.
 */
	bool            is_finished(int) const;

/**
 * checks if the timeout identified by the identifier is still counting to
 * the max value...
 */
	bool            is_running(int) const;

/**
 * checks if the timeout identified by the identifier is paused.
 */

	bool            is_paused(int) const;

	timeout&       pause(int);

/**
 * resumes the given timeout and returns it.
 */
	timeout&       resume(int);

/**
 * restarts the given timeout and returns it.
 */
	timeout&       restart(int);

	private:

	std::map<int, timeout>  data;
};

std::ostream& operator<<(std::ostream&, const timeout&);
}}
