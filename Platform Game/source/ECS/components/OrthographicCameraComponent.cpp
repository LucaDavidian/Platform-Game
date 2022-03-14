#include "OrthographicCameraComponent.h"
//#include "glm/gtc/matrix_transform.hpp"
#include "../../Lua bindings/LuaVM.h"
#include "../Entity.h"

namespace
{
	OrthographicCameraComponent OrthographicCameraComponentTableCtor(LuaObject componentTable)
	{
		LuaObject aspectRatio = componentTable.Get("aspect_ratio");
		LuaObject height = componentTable.Get("height");
		LuaObject nearPlane = componentTable.Get("near_plane");
		LuaObject farPlane = componentTable.Get("far_plane");

		return OrthographicCameraComponent(aspectRatio, height, nearPlane, farPlane);
	}

	void OrthographicCameraComponentAddToEntity(OrthographicCameraComponent const &cameraComponent, Entity &entity)
	{
		entity.AddComponent<OrthographicCameraComponent, CameraComponent>(cameraComponent);
	}
}

const bool OrthographicCameraComponent::mRegistered = []() {
	Reflect::Reflect<OrthographicCameraComponent>("camera_ortho")
		.AddConstructor(OrthographicCameraComponentTableCtor)
		.AddMemberFunction(OrthographicCameraComponentAddToEntity, "add_to_entity");

	return true;
}();

OrthographicCameraComponent::OrthographicCameraComponent(float aspectRatio, float height, float nearPlane, float farPlane)
	: CameraComponent(CameraComponent::Type::ORTHOGRAPHIC, aspectRatio, height, nearPlane, farPlane)
{
}

void OrthographicCameraComponent::UpdateProjectionMatrix()
{
	glm::mat4 projectionMatrix(0.0f); // all zeroes

	float x = 2.0f / (mHeight * mAspectRatio);
	float y = 2.0f / mHeight;
	float za = 2.0f / (mNearPlane - mFarPlane);
	float zb = (mNearPlane + mFarPlane) / (mNearPlane - mFarPlane);

	projectionMatrix[0][0] = x;
	projectionMatrix[1][1] = y;
	projectionMatrix[2][2] = za;
	projectionMatrix[3][2] = zb;
	projectionMatrix[3][3] = 1.0f;

	mProjectionMatrix = projectionMatrix;

	//mProjectionMatrix = glm::ortho(-mHeight * mAspectRatio / 2.0f, mHeight * mAspectRatio / 2.0f, -mHeight / 2.0f, mHeight / 2.0f, mNearPlane, mFarPlane);

	mProjectionDirty = false;
}

