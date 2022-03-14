#include "CameraComponent.h"
#include "../../GraphicsManager.h"
#include "../Entity.h"
#include "TransformComponent.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../../Lua bindings/LuaVM.h"

CameraComponent::CameraComponent(Type type, float aspectRatio, float fov, float nearPlane, float farPlane) 
	: mType(type), mAspectRatio(aspectRatio), mNearPlane(nearPlane), mFarPlane(farPlane), mOffset(glm::vec3()), mRelativeOrientation(glm::vec3())
{
	if (mType == Type::PERSPECTIVE)
		mVerticalFOV = glm::radians(fov);
	else  // mType == Type::ORTHOGRAPHIC
		mHeight = fov;

	mProjectionDirty = true;
	mOffsetDirty = true;
}

glm::mat4 CameraComponent::GetViewMatrix()
{
	TransformComponent *transformComponent = GetOwner()->GetComponent<TransformComponent>();

	glm::mat4 inverseViewMatrix = transformComponent->GetWorldMatrix() * GetOffsetMatrix();

	return glm::inverse(inverseViewMatrix);
}

glm::mat4 CameraComponent::GetInverseViewMatrix()
{
	TransformComponent *transformComponent = GetOwner()->GetComponent<TransformComponent>();

	glm::mat4 inverseViewMatrix = transformComponent->GetWorldMatrix() * GetOffsetMatrix();

	return inverseViewMatrix;
}

glm::mat4 CameraComponent::GetOffsetMatrix()
{
	if (mOffsetDirty)
		UpdateOffsetMatrix();

	return mOffsetMatrix;
}

void CameraComponent::UpdateOffsetMatrix()
{
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), mRelativeOrientation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, mRelativeOrientation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, mRelativeOrientation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), mOffset);

	mOffsetMatrix = translationMatrix * rotationMatrix;

	mOffsetDirty = false;
}

glm::mat4 CameraComponent::GetProjectionMatrix()
{
	if (mProjectionDirty)
		UpdateProjectionMatrix();
	
	return mProjectionMatrix;
}

glm::vec3 CameraComponent::GetRay(unsigned int mouseX, unsigned int mouseY)
{
	TransformComponent *transformComponent = GetOwner()->GetComponent<TransformComponent>();

	glm::vec3 pointOnNearPlane = GetPointOnNearPlane(mouseX, mouseY);

	glm::vec3 worldRay = pointOnNearPlane - (transformComponent->GetPosition() + mOffset);

	return worldRay;
}

glm::vec3 CameraComponent::GetPointOnNearPlane(unsigned int mouseX, unsigned int mouseY)
{
	float ndcX = -1.0f + 2.0f * mouseX / (float)GraphicsManager::GetInstance().GetViewportWidth();
	float ndcY = -1.0f + 2.0f * mouseY / (float)GraphicsManager::GetInstance().GetViewportHeight();

	glm::mat4 inverseViewMatrix = GetInverseViewMatrix();
	glm::mat4 projectionMatrix = GetProjectionMatrix();
	glm::mat4 inverseProjectionMatrix = glm::inverse(projectionMatrix);

	glm::vec4 worldPosition = inverseViewMatrix * inverseProjectionMatrix * glm::vec4(ndcX, ndcY, -1.0f, 1.0f);
	worldPosition.z = 0.0f;

	return worldPosition;
}