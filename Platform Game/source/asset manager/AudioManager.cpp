#include "AudioManager.h"
#include <SDL2/SDL.h>
#include "../logger/Logger.h"

Audio const &AudioManager::GetSound(const std::string &name) 
{
	if (auto it = mSounds.find(name); it == mSounds.end())
		mSounds.insert(std::make_pair(name, Audio(name)));

	return mSounds.at(name);
}

