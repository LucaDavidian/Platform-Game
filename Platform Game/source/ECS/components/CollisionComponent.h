#ifndef COLLISIONCOMPONENT_H
#define COLLISIONCOMPONENT_H

#include "../Component.h"
#include <glm/glm.hpp>

// TODO: a simple AABB collision component for now
// TODO: debug draw colliders

class CollisionComponent : public Component
{
public:
	CollisionComponent(glm::vec2 offset = glm::vec2(0.0f), glm::vec2 dimensions = glm::vec2(1.0f, 1.0f)) : mOffset(offset), mDimensions(dimensions) {}

	glm::vec2 GetOffset() const { return mOffset; }
	glm::vec2 GetDimensions() const { return mDimensions; }

	glm::vec3 GetColor() const { return mColor; }
	void SetColor(glm::vec3 color) { mColor = color; }

	void SetOnGround(bool onGround) { mOnGround = onGround; }  // #TODO this logic goes elsewhere
	bool IsOnGround() const { return mOnGround; }

	void SetTouchingWall(bool touchingWall) { mTouchingWall = touchingWall; }
	bool IsTouchingWall() const { return mTouchingWall; }

private:
	glm::vec2 mOffset;      // the collider's offset and dimensions are scaled by the entity's transform component scale factors
	glm::vec2 mDimensions;

	bool mOnGround;
	bool mTouchingWall;

	glm::vec3 mColor = { 1.0f, 1.0f, 1.0f };
};

#endif // COLLISIONCOMPONENT_H
