#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#include <vector>
#include <glm/glm.hpp>
#include "../../data structures/vector.hpp"
#include "../Component.h"
#include <algorithm>

class TransformAnimationComponent : public Component
{
public:
	class Animation
	{
		friend class TransformAnimationComponent;
	public:
		struct KeyFrame
		{
			glm::vec3 mPosition;
			glm::vec3 mOrientation;
			glm::vec3 mScale;

			float mTime;
		};

		Animation() = default;
		Animation(Vector<KeyFrame> const &keyFrames) : mKeyFrames(keyFrames) {}

	private:

		Vector<KeyFrame> mKeyFrames;

		float mCurrentTime = 0.0f;

		bool mLoop = false;

		glm::vec3 mCurrentPosition;
		glm::vec3 mCurrentOrientation;
		glm::vec3 mCurrentScale;
	};
public:
	bool Update(float dt)
	{
		if (mAnimation.mKeyFrames.Empty())
			return false;

		mAnimation.mCurrentTime += dt / 1000.0f;

		if (mAnimation.mCurrentTime >= mAnimation.mKeyFrames.Last().mTime)
		{
			if (mAnimation.mLoop)
			{
				mAnimation.mCurrentPosition = mAnimation.mKeyFrames.First().mPosition;
				mAnimation.mCurrentOrientation = mAnimation.mKeyFrames.First().mOrientation;
				mAnimation.mCurrentScale = mAnimation.mKeyFrames.First().mScale;

				mAnimation.mCurrentTime = mAnimation.mKeyFrames.First().mTime;
			}
			else
			{
				mAnimation.mCurrentPosition = mAnimation.mKeyFrames.Last().mPosition;
				mAnimation.mCurrentOrientation = mAnimation.mKeyFrames.Last().mOrientation;
				mAnimation.mCurrentScale = mAnimation.mKeyFrames.Last().mScale;

				mAnimation.mKeyFrames.Clear();

				return false;
			}

			return true;
		}

		for (size_t i = 0; i < mAnimation.mKeyFrames.Size() - 1; i++)
		{
			if (mAnimation.mCurrentTime >= mAnimation.mKeyFrames[i].mTime && mAnimation.mCurrentTime < mAnimation.mKeyFrames[i + 1].mTime)
			{
				float t = (mAnimation.mCurrentTime - mAnimation.mKeyFrames[i].mTime) / (mAnimation.mKeyFrames[i + 1].mTime - mAnimation.mKeyFrames[i].mTime);

				mAnimation.mCurrentPosition = Lerp(mAnimation.mKeyFrames[i].mPosition, mAnimation.mKeyFrames[i + 1].mPosition, t);
				mAnimation.mCurrentOrientation = Lerp(mAnimation.mKeyFrames[i].mOrientation, mAnimation.mKeyFrames[i + 1].mOrientation, t);
				mAnimation.mCurrentScale = Lerp(mAnimation.mKeyFrames[i].mScale, mAnimation.mKeyFrames[i + 1].mScale, t);

				return true;
			}
		}
	}

	glm::vec3 GetCurrentPosition() const
	{
		return mAnimation.mCurrentPosition;
	}

	glm::vec3 GetCurrentOrientation() const
	{
		return mAnimation.mCurrentOrientation;
	}

	glm::vec3 GetCurrentScale() const
	{
		return mAnimation.mCurrentScale;
	}

	void PlayAnimation(Animation animation, bool loop = false)
	{
		std::sort(animation.mKeyFrames.Begin(), animation.mKeyFrames.End(), [](auto kf1, auto kf2) { return kf1.mTime < kf2.mTime; });

		mAnimation = animation;
		mAnimation.mCurrentTime = 0.0f;
		mAnimation.mLoop = loop;
	}

private:
	glm::vec3 Lerp(glm::vec3 a, glm::vec3 b, float t)
	{
		return (b - a) * t + a;
	}

	Animation mAnimation;

	const static bool mRegistered;
};

#endif  // ANIMATION_COMPONENT_H


