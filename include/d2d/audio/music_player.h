#pragma once

#include <dfw/audio.h>
#include <lda/resource_manager.h>
#include <lda/audio_callback_interface.h>
#include <lm/logger.h>

namespace d2d { namespace audio {

/**
 * simple class to control music. A change of piece can be requested so the
 * old piece will stop playing and the new will start. The old piece will
 * ramp down and whichever one has been requested last will stop.
 */
class music_player :
	public lda::audio_callback_interface {

	public:

	                            music_player(lm::logger&, dfw::audio&, lda::resource_manager&);
	                            ~music_player();
/**
 * adds the next piece (by id) to be played. It won't neccessarily start 
 * playing at the moment this method is called: if there is a piece playing
 * it will ramp the volume down in 300ms and then start playing the new piece
 * with a 300ms ramp up.
 */
	void                        enqueue(int);
/**
 * will pause the music
 */
	void                        disable();
/**
 * will resume the music
 */
	void                        enable();
/**
 * will start playing the next piece (if any) with a ramp up of 300ms).
 */
	void                        play_next();
/**
 * returns true if there is an enqueued piece.
 */
	bool                        must_play_next() const {return -1==playing_id;}
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

	//references
	lm::logger&                 log;
	dfw::audio&                 audio;
	lda::resource_manager&      audio_resource_manager;

	//properties
	bool                        enabled{false};
	int                         playing_id{0},
	                            next_id{0};
};

}}
