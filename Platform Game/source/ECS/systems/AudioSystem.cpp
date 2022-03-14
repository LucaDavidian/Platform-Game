#include "AudioSystem.h"
#include <SDL2/SDL.h>
//#include <SDL mixer/SDL_mixer.h>
#include "../../logger/Logger.h"
#include "../../asset manager/AudioManager.h"

AudioSystem::AudioSystem()
{
	if (SDL_Init(SDL_INIT_AUDIO))
		Logger::Instance().Log(std::string("can't initialize SDL audio: ") + SDL_GetError(), Logger::Level::LOG_ERROR);

	SDL_AudioSpec audioDeviceRequest;
	audioDeviceRequest.freq = 44100;
	audioDeviceRequest.format = AUDIO_S16;
	audioDeviceRequest.samples = 4096;
	audioDeviceRequest.channels = 2;
	audioDeviceRequest.callback = NULL;  // using SDL_QueueAudio
	audioDeviceRequest.userdata = NULL;  // ignored
 	audioDeviceRequest.size;             // set by SDL_OpenAudioDevice
	audioDeviceRequest.silence;          // set by SDL_OpenAudioDevice
	
	//SDL_zero(audioDeviceRequest);

	if ((mAudioDeviceId = SDL_OpenAudioDevice(NULL, 0, &audioDeviceRequest, &mAudioDeviceSpec, 0)) == 0)
		Logger::Instance().Log(std::string("can't open audio device: " ) + SDL_GetError(), Logger::Level::LOG_ERROR);
}

AudioSystem::~AudioSystem()
{
	SDL_CloseAudioDevice(mAudioDeviceId);
}

void AudioSystem::PlayAudio(const std::string name)
{
	AudioManager::GetInstance().GetSound(name).Play(mAudioDeviceId);	
}