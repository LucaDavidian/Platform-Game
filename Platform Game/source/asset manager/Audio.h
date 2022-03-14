#ifndef AUDIO_H
#define AUDIO_H

#include <string>
#include <cstdint>
#include <SDL2/SDL.h>

class Audio
{
public:
	Audio(const std::string &name);
	
	Audio(const Audio &other);
	Audio(Audio &&other);

	~Audio();

	Audio &operator=(const Audio &other);
	Audio &operator=(Audio &&other);

	void Play(SDL_AudioDeviceID audioDeviceID) const;
private:
	void Load(const std::string &fileName);

	std::string mName;
	SDL_AudioSpec mWavSpec;
	uint8_t *mBuffer;
	uint32_t mLength;
};

#endif // AUDIO_H