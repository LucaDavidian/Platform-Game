#ifndef SPRITEANIMATION_COMPONENT_H
#define SPRITEANIMATION_COMPONENT_H

#include <map>
#include <string>
#include "../Component.h"
#include "../../data structures/vector.hpp"
#include "../../event system/EventBus.hpp"
#include "../../event system/Event.hpp"
#include "../Entity.h"

class SpriteAnimationComponent : public Component
{
public:
	struct Animation
	{
		struct KeyFrame
		{
			unsigned int mTileNum;
			float mDurationSec;
		};

		Vector<KeyFrame> mKeyFrames;
		unsigned int mCurrentKeyFrame = 0;

		bool mLoop = false;
	};

public:
	void AddAnimation(const std::string &animationName, Animation animation)
	{ 
		mAnimations.insert(std::make_pair(animationName, animation));
	}

	void PlayAnimation(const std::string &animationName, bool loop = false)
	{
		auto it = mAnimations.find(animationName);

		if (it != mAnimations.end())
		{
			mCurrentAnimation = it->first;
			mAnimations[mCurrentAnimation].mCurrentKeyFrame = 0;
			mAnimations[mCurrentAnimation].mLoop = loop;
		}
		else
			mCurrentAnimation = "";
	}

	std::string const &GetCurrentAnimation() const
	{
		return mCurrentAnimation;
	}

	unsigned int GetCurrentTile() const
	{
		return mAnimations.at(mCurrentAnimation).mKeyFrames[mAnimations.at(mCurrentAnimation).mCurrentKeyFrame].mTileNum;
	}

	bool Update(float dt)
	{
		if (mCurrentAnimation.empty())
			return false;

		Animation &currentAnimation = mAnimations[mCurrentAnimation];

		mCurrentFrameTime += dt / 1000.0f;

		if (mCurrentFrameTime > currentAnimation.mKeyFrames[currentAnimation.mCurrentKeyFrame].mDurationSec)
		{
			currentAnimation.mCurrentKeyFrame++;
			mCurrentFrameTime = 0.0f;

			if (currentAnimation.mCurrentKeyFrame >= currentAnimation.mKeyFrames.Size())
				if (currentAnimation.mLoop)
					currentAnimation.mCurrentKeyFrame = 0;
				else
				{
					mCurrentAnimation = "";
					
					GetOwner()->GetEventBus().TriggerEvent<SpriteAnimationEndEvent>();

					return  false;
				}

			return true;
		}
		
		if (currentAnimation.mCurrentKeyFrame == 0)
			return true;

		return false;
	}

private:
	std::map<std::string, Animation> mAnimations;
	std::string mCurrentAnimation;
	float mCurrentFrameTime = 0.0f;

	static const bool mRegistered;
};

#endif  // SPRITEANIMATION_COMPONENT_H