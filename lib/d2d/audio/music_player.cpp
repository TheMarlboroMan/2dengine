#include "d2d/audio/music_player.h"
#include <lm/log.h>

using namespace d2d::audio;

music_player::music_player(
	lm::logger& _log,
	dfw::audio& _audio,
	lda::resource_manager& _arm
):
	log{_log},
	audio{_audio},
	audio_resource_manager{_arm}
{

	audio().add_music_stop_callback("d2d::music_player", *this);
}

music_player::~music_player() {

	audio().remove_music_stop_callback("d2d::music_player");
}

void music_player::enqueue(
	int _music_id
) {

	next_id=_music_id;
	lm::log(log).debug()<<"requesting to play music "<<_music_id<<std::endl;

	if(_music_id==playing_id) {

		if(next_id==_music_id) {

			lm::log(log).debug()<<"will play the same currently played piece"<<std::endl;
		}
		else {

			lm::log(log).debug()<<"will not play, as it is the currently played piece"<<std::endl;
			return;
		}

	}

	if(audio().is_music_playing()) {

		//TODO: Should be parametrizable.
		audio().stop_music(300);
	}
	else {

		//Consider that the first piece might be silence.
		if(_music_id) {

			playing_id=_music_id;
			//TODO: Again, should be parametrizable.
			audio().play_music({audio_resource_manager.get_music(playing_id), 300});
		}
	}
}

void music_player::do_callback() {

	lm::log(log).debug()<<"music_player do_callback for "<<playing_id<<std::endl;

	if(-1!= playing_id) {

		//maybe we are getting called at the same time the audio_menu class is
		//getting called too.
		if(!audio_resource_manager.has_music(playing_id)) {

			lm::log(log).debug()<<"will not unload non-existing music "<<playing_id<<std::endl;
			return;
		}

		//New music got requested and the same one we are playing got requested
		//before that one faded out, so we keep the one we have...
		if(playing_id==next_id) {

			lm::log(log).debug()<<"will not unload music "<<playing_id<<", it is next in queue"<<std::endl;
			playing_id=-1;
			return;
		}

		//Free music just got played...
		lm::log(log).debug()<<"unloading finished music "<<playing_id<<std::endl;
		audio_resource_manager.unload_music(playing_id);
		lm::log(log).debug()<<"remaining musics: "<<audio_resource_manager.size_music()<<std::endl;
	}

	playing_id=-1;
}

void music_player::play_next() {

	playing_id=next_id;

	if(!playing_id) {

		return;
	}

	audio().play_music({audio_resource_manager.get_music(playing_id), 300});
}

void music_player::disable() {

	enabled=false;
	audio().pause_music();
}


void music_player::enable() {

	enabled=true;
	audio().resume_music();
}
