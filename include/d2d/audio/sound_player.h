#pragma once

#include "sound_player_then.h"
#include <lda/resource_manager.h>
#include <dfw/audio.h>
#include <lm/logger.h>

namespace d2d { namespace audio {

class sound_player {

	public:
	                        sound_player(lm::logger&, dfw::audio&, const lda::resource_manager&);

	//Just a convenience.
	dfw::audio&             operator()(){return audio;}
/**
* Quick fire and forget.
* attempts to play the sound given by id in a free channel. Returns
* the channel index it is playing on or a negative number on any error.
*/
	int                     play_once(int);
/**
*Same as above, but repeating the sound. The second parameter controls the
*repeats, -1 being infinite.
 */
	int                     play_repeat(int, int=-1);

/**
 *Plays a sound and executes the thenable when it stops.
 */
	int                     play_once_then(int, sound_player_then&);
/**
*This is just a passthrough to the library to stop a playing sound. It must
*get the channel of the playing sound, which is returned by any of the
*play_sound methods.
*/
	void                    stop(int);

	private:

	int                     play_sound(int, int);

	lm::logger&             log;
	dfw::audio&             audio;
	const lda::resource_manager&  audio_resource_manager;

};
}}
