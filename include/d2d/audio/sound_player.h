#pragma once

#include <lda/resource_manager.h>
#include <dfw/audio.h>
#include <lm/logger.h>

namespace d2d { namespace audio {

class sound_player {

	public:
	                        sound_player(lm::logger&, dfw::audio&, const lda::resource_manager&);
/**
 * attempts to play the sound given by id in a free channel.
 */
	void                    play_sound(int);

	private:

	lm::logger&             log;
	dfw::audio&             audio;
	const lda::resource_manager&  audio_resource_manager;

};

}}
