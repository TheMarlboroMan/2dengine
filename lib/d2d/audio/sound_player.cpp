#include "d2d/audio/sound_player.h"

#include <lda/audio_channel.h>
#include <lda/sound.h>
#include <lda/exception.h>
#include <lm/log.h>
#include <stdexcept>

using namespace d2d::audio;

sound_player::sound_player(
	lm::logger& _logger,
	dfw::audio& _audio,
	const lda::resource_manager& _arm
):
	log{_logger},
	audio{_audio},
	audio_resource_manager{_arm}
{}

int sound_player::play_once(
	int _sound_id
) {

	return play_sound(_sound_id, 0);
}

int sound_player::play_repeat(
	int _sound_id,
	int _repeat
) {

	return play_sound(_sound_id, _repeat);
}

int sound_player::play_sound(
	int _sound_id,
	int _repeats
) {

	if(!audio().has_free_channels()) {

		return -1;
	}

	lda::audio_channel_safe channel{audio().get_free_channel()};
	if(!audio_resource_manager.has_sound(_sound_id)) {

		lm::log(log).warning()<<"unable to play unknown sound "<<_sound_id<<std::endl;
		return -2;
	}

	lda::sound_struct snd{audio_resource_manager.get_sound(_sound_id), -1, _repeats};

	channel.play(snd);
	return channel.get_index();
}

int sound_player::play_once_then(
	int _sound_id,
	sound_player_then& _then
) {

	int index=play_once(_sound_id);
	if(0 > index) {

		return index;
	}

	auto channel=audio().get_channel(index);
	_then.ready(channel);
	_then.on_start();
	return index;
}

void sound_player::stop(
	int _channel_id
) {

	audio().stop_sound(_channel_id);
}
