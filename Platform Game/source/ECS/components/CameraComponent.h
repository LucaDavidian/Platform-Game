#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#include "../Component.h"
#include "glm/glm.hpp"

class CameraComponent : public Component
{	
public:
	struct Frustum
	{
		glm::vec3 nearTopLeft;
		glm::vec3 nearTopRight;
		glm::vec3 nearBottomLeft;
		glm::vec3 nearBottomRight;
		glm::vec3 farTopLeft;
		glm::vec3 farTopRight;
		glm::vec3 farBottomLeft;
		glm::vec3 farBottomRight;
	};
	enum class Type { PERSPECTIVE, ORTHOGRAPHIC };
public:
	void SetLens(float aspectRatio, float verticalFOV, float nearPlane, float farPlane);  // dirty

	void SetOffset(const glm::vec3 &offset) { mOffset = offset; mOffsetDirty = true; }
	glm::vec3 GetOffset() const { return mOffset; }

	void SetRelativeOrientation(float yaw, float pitch, float roll) { mRelativeOrientation = glm::vec3(yaw, pitch, roll); mOffsetDirty = true; }
	glm::vec3 GetRelativeOrientation() { return mRelativeOrientation; }

	glm::mat4 GetViewMatrix();
	glm::mat4 GetInverseViewMatrix();
	glm::mat4 GetProjectionMatrix();

	float GetNearPlane() const { return mNearPlane; }
	float GetFarPlane() const { return mFarPlane; }

	Type GetType() const { return mType; }
	const Frustum &GetFrustum() const { return mFrustum; }

	glm::vec3 GetPointOnNearPlane(unsigned int mouseX, unsigned int mouseY);
	glm::vec3 GetRay(unsigned int mouseX, unsigned int mouseY);
protected:
	CameraComponent(Type type, float aspectRatio, float fov, float nearPlane, float farPlane);

	Type mType;
	Frustum mFrustum;

	glm::mat4 GetOffsetMatrix();
	void UpdateOffsetMatrix();
	bool mOffsetDirty;
	glm::vec3 mOffset;
	glm::vec3 mRelativeOrientation;
	glm::mat4 mOffsetMatrix;

	glm::mat4 mProjectionMatrix;

	virtual void UpdateProjectionMatrix() = 0; 
	bool mProjectionDirty;

	union
	{
		float mVerticalFOV;     // for perspective projection camera
		float mHeight;          // for orthographic projection camera
	};
	
	float mAspectRatio;         // aspect ratio = width/height
	float mNearPlane;
	float mFarPlane;
};

#endif  // CAMERA_COMPONENT_H
