#pragma once

#include <lda/audio_callback_interface.h>
#include <lda/audio_channel.h>

namespace d2d { namespace audio {

class sound_player_then : lda::audio_callback_interface {

	public:

	/** Will execute once the sound starts. */
	virtual void        on_start()=0;
	/** Will execute once the sound ends. */
	virtual void        on_end()=0;

	void                do_callback();
	void                ready(lda::audio_channel _channel);

	private:

	lda::audio_channel channel;
};
}}

