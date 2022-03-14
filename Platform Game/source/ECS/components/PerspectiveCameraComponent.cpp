#include "PerspectiveCameraComponent.h"
//#include "glm/gtc/matrix_transform.hpp"

PerspectiveCameraComponent::PerspectiveCameraComponent(float aspectRatio, float verticalFOV, float nearPlane, float farPlane)
	: CameraComponent(CameraComponent::Type::PERSPECTIVE, aspectRatio, verticalFOV, nearPlane, farPlane)
{
}

void PerspectiveCameraComponent::UpdateProjectionMatrix()
{
	glm::mat4 projectionMatrix(0.0); // all zeroes

	float x = 1.0f / (2.0f * mAspectRatio * glm::tan(mVerticalFOV / 2.0f));
	float y = 1.0f / (2.0f * glm::tan(mVerticalFOV / 2.0f));
	float za = (mNearPlane + mFarPlane) / (mNearPlane - mFarPlane);
	float zb = 2.0f * mNearPlane * mFarPlane / (mNearPlane - mFarPlane);

	projectionMatrix[0][0] = x;
	projectionMatrix[1][1] = y;
	projectionMatrix[2][2] = za;
	projectionMatrix[3][2] = zb;
	projectionMatrix[2][3] = -1.0f;

	mProjectionMatrix = projectionMatrix;

	//mProjectionMatrix = glm::perspectiveFov(mVerticalFOV, mAspectRatio * 1.0f, 1.0f, mNearPlane, mFarPlane);  

	mProjectionDirty = false;
}