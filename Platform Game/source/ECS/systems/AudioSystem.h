#ifndef AUDIO_SYSTEM_H
#define AUDIO_SYSTEM_H

#include <SDL2/SDL_audio.h>
#include <string>

class AudioSystem
{
public:
	static AudioSystem &GetInstance() 
	{ 
		static AudioSystem instance; 
		return instance; 
	}

	void PlayAudio(const std::string name);
private:
	AudioSystem();
	~AudioSystem();

	SDL_AudioSpec mAudioDeviceSpec;
	SDL_AudioDeviceID mAudioDeviceId;
};

#endif // AUDIO_SYSTEM_H
