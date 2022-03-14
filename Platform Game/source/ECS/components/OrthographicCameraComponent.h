#ifndef ORTHOGRAPHIC_CAMERA_COMPONENT_H
#define ORTHOGRAPHIC_CAMERA_COMPONENT_H

#include "CameraComponent.h"

class OrthographicCameraComponent : public CameraComponent
{
public:
	OrthographicCameraComponent(float aspectRatio, float height, float nearPlane, float farPlane);
private:
	void UpdateProjectionMatrix() override;

	const static bool mRegistered;
};

#endif  // ORTHOGRAPHIC_CAMERA_COMPONENT_H
