#ifndef PERSPECTIVE_CAMERA_COMPONENT_H
#define PERSPECTIVE_CAMERA_COMPONENT_H

#include "CameraComponent.h"

class PerspectiveCameraComponent : public CameraComponent
{
public:
	PerspectiveCameraComponent(float aspectRatio, float verticalFOV, float nearPlane, float farPlane);
private:
	void UpdateProjectionMatrix() override;
};

#endif  // PERSPECTIVE_CAMERA_COMPONENT_H
