#ifndef GUIHEALTHBAR_H
#define GUIHEALTHBAR_H

#include "GUIComponent.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../ECS/systems/RenderSystem.h"

class GUIHealthBar : public GUIComponent
{
public:
	void Draw() override
	{
		RenderSystem::GetInstance().SetShader(RenderSystem::GetInstance().mGUIShader);

		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(mNormalizedDimensions.x, mNormalizedDimensions.y, 0.0f));
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(GetNormalizedPositionCenter().x, GetNormalizedPositionCenter().y, 0.0f));
		glm::mat4 modelMatrix = translationMatrix * scaleMatrix;

		RenderSystem::GetInstance().GetCurrentShader().SetUniform("hasTexture", false);
		RenderSystem::GetInstance().GetCurrentShader().SetUniform("isOver", false);

		RenderSystem::GetInstance().GetCurrentShader().SetUniform("modelMatrix", modelMatrix);
		RenderSystem::GetInstance().GetCurrentShader().SetUniform("color", mBackgroundColor.r, mBackgroundColor.g, mBackgroundColor.b);

		RenderSystem::GetInstance().GetQuadMesh().Bind();
		RenderSystem::GetInstance().GetQuadMesh().DrawTriangles();

		scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(mNormalizedDimensions.x * 0.9f, mNormalizedDimensions.y * 0.95f, 0.0f));
		modelMatrix = translationMatrix * scaleMatrix;

		RenderSystem::GetInstance().GetCurrentShader().SetUniform("modelMatrix", modelMatrix);
		RenderSystem::GetInstance().GetCurrentShader().SetUniform("color", mBarColor.r, mBarColor.g, mBarColor.b);
		
		RenderSystem::GetInstance().GetQuadMesh().Bind();
		RenderSystem::GetInstance().GetQuadMesh().DrawTriangles();
	}
private:
	glm::vec3 mBarColor{ 1.0f, 0.0f, 0.0f };
	glm::vec3 mBackgroundColor{ 0.0f, 0.0f, 1.0f };
	float mHealth;
};

#endif // GUIHEALTHBAR_H
