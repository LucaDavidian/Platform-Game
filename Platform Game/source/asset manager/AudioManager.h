#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <map>
#include <string>
#include <cstdint>
#include "Audio.h"

class AudioManager
{
public:
	static AudioManager& GetInstance()
	{
		static AudioManager instance;
		return instance;
	}

	Audio const &GetSound(const std::string &name);
private:
	AudioManager() = default;

	std::map<std::string, Audio> mSounds;
};

#endif // AUDIO_MANAGER_H
