#pragma once

#include <dfw/audio.h>
#include <lda/resource_manager.h>
#include <lda/audio_callback_interface.h>
#include <lm/logger.h>
#include <functional>

namespace d2d { namespace audio {

/**
 * simple class to control music. It understands music in two "slots", the one
 * that is "currently" playing and the one will be be played "next" on request.
 * This is by no means a playlist, it just means that this class can swap the
 * currently played piece by another one, ramping them down and up.
 * Memory wise it assumes the pieces exist in memory and will not attempt to
 * unload them when finished. Howhever, a load function can be specified to 
 * load non-existing pieces and the set_unload_finished can be used to 
 * unload memory after music is done playing.
 */
class music_player :
	public lda::audio_callback_interface {

	public:

	                            music_player(lm::logger&, dfw::audio&, lda::resource_manager&);
	                            ~music_player();

/**
 * request the immediate start of the piece with the given id using a ramp up
 * of N ms. If zero is given as the id no music will be played. Resets the
 * current piece slot and clears the next piece slot. If silence is
 * requested, the fade time will be used as fadeout.
 */

	void                        play(int, int=0);
/**
 * requests the currently piece to stop using a ramp down of N ms. The current
 * and next pieces are not reset.
 */
	void                        stop(int=0);
/**
 * requests the current piece to pause.
 */
	void                        pause();
/**
 * requests the current piece to resume after being paused.
 */
	void                        resume();

/**
 * requests the current piece to stop playing with a fadeout of N
 * ms and for it to be replaced with the given piece (zero means
 * silence). More calls to "swap" can be made while the fadeout
 * takes place, which will change the "next" piece. Only after the
 * fadeout completes will the "current" piece slot change.
 */
	void                        swap(int, int=100);
/**
 * will pause the music
 */
	void                        disable();
/**
 * will resume the music
 */
	void                        enable();

/**
 * sets if music should be unloaded from memory after it finishes playing.
 */
	void                        set_unload_finished(bool _val) {

		unload_finished=_val;
	}

/**
 * sets the load callback method.
 */
	void                        set_load_music_function(std::function<std::string(int)> _fn) {

		load_fn=_fn;
	}
/**
 * returns true if there is an enqueued piece.
 */
	bool                        must_play_next() const {return 0==playing_id;}
/**
 * returns the id of the next piece
 */
	int                         get_next() const {return next_id;}
/**
 * returns the id of the current peice
 */
	int                         get_current() const {return playing_id;}
/**
 * returns true if the music is playing
 */
	bool                        is_enabled() const {return enabled;}

	//implementation of lda::audio_callback_interface
	void do_callback();

	//end implementation of lda::audio_callback_interface

	private:

	void                        load_attempt();

	//references
	lm::logger&                 log;
	dfw::audio&                 audio;
	lda::resource_manager&      audio_resource_manager;

	//properties
	bool                        enabled{false},
	                            unload_finished{false};
	int                         playing_id{0},
	                            next_id{-1},
	                            fade{0};
	std::function<std::string(int)> load_fn;
};

}}
