#include "Audio.h"
#include "../logger/Logger.h"

Audio::Audio(const std::string &name) : mName(name), mWavSpec{}, mBuffer(nullptr), mLength(0U)
{
	Load("assets/audio/" + mName + ".wav");
}

Audio::Audio(const Audio &other) : mName(), mWavSpec{}, mBuffer(nullptr), mLength(0U)
{
	mName = other.mName;

	if (!mName.empty())
		Load("assets/audio/" + mName + ".wav");
}

Audio::Audio(Audio &&other) : mName(other.mName), mWavSpec(other.mWavSpec), mBuffer(other.mBuffer), mLength(other.mLength)
{
	other.mBuffer = nullptr;
}

Audio::~Audio()
{
	SDL_FreeWAV(mBuffer);
}

Audio &Audio::operator=(const Audio &other)
{
	Audio temp(other);
	std::swap(*this, temp);

	return *this;
}

Audio &Audio::operator=(Audio &&other)
{
	Audio temp(std::move(other));
	std::swap(*this, temp);

	return *this;
}

void Audio::Load(const std::string &fileName)
{
	if (!SDL_LoadWAV(fileName.c_str(), &mWavSpec, &mBuffer, &mLength))
		Logger::Instance().Log("can't load audio file " + fileName + ": " + SDL_GetError(), Logger::Level::LOG_ERROR);
}

void Audio::Play(SDL_AudioDeviceID audioDeviceID) const
{
	if (SDL_QueueAudio(audioDeviceID, mBuffer, mLength))
		Logger::Instance().Log(("can't play audio " + mName + ": " + SDL_GetError()).c_str(), Logger::Level::LOG_ERROR);

	SDL_PauseAudioDevice(audioDeviceID, 0); 
}

