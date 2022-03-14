#ifndef LIVESCOMPONENT_H
#define LIVESCOMPONENT_H

#include "../../event system/EventBus.hpp"
#include "../../event system/Event.hpp"

class LivesComponent : public Component
{
public:
	LivesComponent(unsigned numLives = 3) : mNumLives(numLives) 
	{
		gEventBus.TriggerEvent<NumLivesChangedEvent>(numLives);
	}

	unsigned GetNumLives() const { return mNumLives; }
	void SetNumLives(unsigned numLives) 
	{ 
		mNumLives = numLives; 
		gEventBus.TriggerEvent<NumLivesChangedEvent>(mNumLives); 
	}

	void DecreaseNumLives() 
	{ 
		if (mNumLives)
		{
			mNumLives--;
			gEventBus.TriggerEvent<NumLivesChangedEvent>(mNumLives);
		}
	}
	void IncreaseNumLives() { mNumLives++; gEventBus.TriggerEvent<NumLivesChangedEvent>(mNumLives); }
private:
	unsigned int mNumLives;

	static const bool mRegistered;
};

#endif // LIVESCOMPONENT_H
