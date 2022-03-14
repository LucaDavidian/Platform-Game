#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "../Component.h"
#include <glm/glm.hpp>

class TransformComponent : public Component
{
public:
	TransformComponent();

	void SetPosition(float positionX, float positionY, float positionZ) { mPosition = glm::vec3(positionX, positionY, positionZ); mDirty = true; }
	void SetPosition(const glm::vec3 &position) { mPosition = position; mDirty = true; }
	glm::vec3 GetPosition() const { return mPosition; }

	void SetOrientation(float yaw, float pitch, float roll) { mOrientation = glm::vec3(yaw, pitch, roll); mDirty = true; }
	void SetOrientation(const glm::vec3 &orientation) { mOrientation = orientation; mDirty = true; }
	glm::vec3 GetOrientation() const { return mOrientation; }

	void SetScale(float scaleX, float scaleY, float scaleZ) { mScale = glm::vec3(scaleX, scaleY, scaleZ); mDirty = true; }
	void SetScale(const glm::vec3 &scale) { mScale = scale; mDirty = true; }
	glm::vec3 GetScale() const { return mScale; }

	glm::mat4 GetWorldMatrix();
	glm::mat4 GetScaledWorldMatrix();
	glm::mat4 GetInverseWorldMatrix();

	glm::vec3 GetAxisX();
	glm::vec3 GetAxisY();
	glm::vec3 GetAxisZ();

private:
	glm::vec3 mPosition;
	glm::vec3 mOrientation;
	glm::vec3 mScale;

	glm::mat4 mWorldMatrix;
	glm::mat4 mScaledWorldMatrix;
	glm::mat4 mInverseWorldMatrix;

	bool mDirty = false;
	void UpdateWorldMatrix();

	const static bool mRegistered;
};

#endif  // TRANSFORM_COMPONENT_H