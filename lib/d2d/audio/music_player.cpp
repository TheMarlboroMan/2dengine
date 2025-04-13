#include "d2d/audio/music_player.h"
#include <lm/log.h>
#include <stdexcept>

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

	lm::log(log).info()<<"unmounting music player, stopping music...\n";
	audio().stop_music();
	lm::log(log).info()<<"unmounting music player, unregistering callback...\n";
	audio().remove_music_stop_callback("d2d::music_player");
}

void music_player::play(
	int _piece_id,
	int _fade
) {

	playing_id=_piece_id;
	fade=_fade;
	next_id=0;

	lm::log(log).debug()<<"<d2d::audio::music_player> requesting audio controller to play music id="<<playing_id<<" with fadein of "<<fade<<std::endl;

	if(0==playing_id) {

		audio().stop_music(fade);
		return;
	}

	load_attempt();
	audio().play_music({audio_resource_manager.get_music(playing_id), fade});
}

void music_player::stop(
	int _fade
) {

	audio().stop_music(_fade);
}

void music_player::pause() {

	audio().pause_music();
}

void music_player::resume() {

	audio().resume_music();
}

void music_player::swap(
	int _music_id_to_enqueue,
	int _fade
) {

	lm::log(log).debug()<<"<d2d::audio::music_player> requesting to swap current "<<playing_id<<" to "<<_music_id_to_enqueue<<std::endl;

	if(_music_id_to_enqueue==playing_id) {

		next_id=_music_id_to_enqueue;
		lm::log(log).debug()<<"<d2d::audio::music_player> asked for the same playing piece, will play after ramp down"<<std::endl;
		return;
	}

	next_id=_music_id_to_enqueue;
	fade=_fade;

	//If there is music, we must stop it and trust the callback to play for us.
	if(audio().is_music_playing()) {

		lm::log(log).debug()<<"<d2d::audio::music_player> requesting audio controller to stop music with fadeout of "<<fade<<std::endl;
		audio().stop_music(fade);
		return;
	}

	//If there was no music playing we are here, so just play a tune.
	play(_music_id_to_enqueue, fade);
}

void music_player::do_callback() {
	
	//These are callbacks for when the music STOPS playing so we can free
	//the associated memory.
	lm::log(log).debug()<<"<d2d::audio::music_player> music_player do_callback for "<<playing_id<<std::endl;

	//We were playing "silence" and swapped it for... something?
	if(0==playing_id) {

		if(next_id==0) {

			return;
		}
	}

	if(!audio_resource_manager.has_music(playing_id)) {

		lm::log(log).debug()<<"<d2d::audio::music_player> will not unload non-existing music "<<playing_id<<std::endl;
		return;
	}

	if(unload_finished) {

		//New music got requested and the same one we are playing got requested
		//before that one faded out, so we keep the one we have...
		if(playing_id==next_id) {

			lm::log(log).debug()<<"<d2d::audio::music_player> will not unload music "<<playing_id<<", it is next in queue"<<std::endl;
		}
		else {

			//Free music just got played...
			lm::log(log).debug()<<"<d2d::audio::music_player> unloading finished music "<<playing_id<<std::endl;
			audio_resource_manager.unload_music(playing_id);
			lm::log(log).debug()<<"<d2d::audio::music_player> remaining musics: "<<audio_resource_manager.size_music()<<std::endl;
		}
	}

	//This will clear out "next_id".
	play(next_id, fade);
	return;
}

void music_player::disable() {

	enabled=false;
	pause();
}

void music_player::enable() {

	enabled=true;
	resume();
}

void music_player::load_attempt() {

	//Last chance to load the music in real time...
	if(!audio_resource_manager.has_music(playing_id)) {

		if(!load_fn) {

			lm::log(log).debug()<<"<d2d::audio::music_player> piece does not exist, no load function specified, will throw"<<std::endl;
			throw std::runtime_error("requested non-loaded music");
		}

		lm::log(log).debug()<<"<d2d::audio::music_player> piece does not exist, will attempt to load"<<std::endl;
		auto music_path=load_fn(playing_id);
		audio_resource_manager.insert_music(playing_id, music_path);

		if(!audio_resource_manager.has_music(playing_id)) {

			lm::log(log).debug()<<"<d2d::audio::music_player> music load failed, will throw"<<std::endl;
			throw std::runtime_error("unable to load music");
		}
	}
}
