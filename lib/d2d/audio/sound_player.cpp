#include "d2d/audio/sound_player.h"

#include <lda/audio_channel.h>
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

void sound_player::play_sound(
	int _sound_id
) {

	lda::audio_channel_safe channel{audio().get_free_channel()};
	if(!channel.is_linked()) {

		lm::log(log).warning()<<"unable to play sound "<<_sound_id<<", all channels busy!"<<std::endl;
		return;
	}

	if(!audio_resource_manager.has_sound(_sound_id)) {

		lm::log(log).warning()<<"unable to play unknown sound "<<_sound_id<<std::endl;
		return;
	}

	audio.play_sound(audio_resource_manager.get_sound(_sound_id));
}
