#include <d2d/audio/sound_player_then.h>

using namespace d2d::audio;

void sound_player_then::do_callback() {

	on_end();
	channel.clear_callback_listener();
}

void sound_player_then::ready(
	lda::audio_channel _channel
) {

	channel=_channel;
	channel.assign_callback_listener(*this);
}

