#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "../Component.h"
#include <glm/glm.hpp>

class PhysicsComponent : public Component
{
public:
	PhysicsComponent(float mass = 0.0f) : mMass(mass) {}

	void SetVelocity(glm::vec2 velocity)
	{
		mVelocity = velocity;
	}

	glm::vec2 GetVelocity() const
	{
		return mVelocity;
	}

	void SetMass(float mass) { mMass = mass; }
	float GetMass() const { return mMass; }
	bool HasMass() const { return mMass; }
private:
	glm::vec2 mVelocity;
	bool mMass;

	const static bool mRegistered;
};

#endif // PHYSICSCOMPONENT_H
