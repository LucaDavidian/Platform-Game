#include "GUIPanel.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "../ECS/systems/RenderSystem.h"
#include "../GraphicsManager.h"

void GUIPanel::Draw()
{
	RenderSystem::GetInstance().SetShader(RenderSystem::GetInstance().mGUIShader);

	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(mNormalizedDimensions.x, mNormalizedDimensions.y, 0.0f));
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(GetNormalizedPositionCenter().x, GetNormalizedPositionCenter().y, 0.0f));
	glm::mat4 modelMatrix = translationMatrix * scaleMatrix;

	RenderSystem::GetInstance().GetCurrentShader().SetUniform("modelMatrix", modelMatrix);
	RenderSystem::GetInstance().GetCurrentShader().SetUniform("color", mColor.r, mColor.g, mColor.b);
	RenderSystem::GetInstance().GetCurrentShader().SetUniform("opacity", mOpacity);

	GraphicsManager::GetInstance().EnableBlending();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	RenderSystem::GetInstance().GetQuadMesh().Bind();
	RenderSystem::GetInstance().GetQuadMesh().DrawTriangles();
	GraphicsManager::GetInstance().DisableBlending();

	GUIComposite::Draw();
}

bool GUIPanel::OnMouseMove(int x, int y, int dx, int dy)
{
	if (mSelectedComponent)
	{
		if (mSelectedComponent->OnMouseMove(x, y, dx, dy))
			return true;
	}
	else
		for (auto &&component : mComponents)
			if (component->OnMouseMove(x, y, dx, dy))
				return true;

	if (IsInside(x, y))  
	{
		if (!mIsFocused)  // on mouse enter
			mIsFocused = true;
	}
	else  
	{
		if (mIsFocused)  // on mouse leave
		{
			mIsFocused = false;
			//InteractionFSM::GetInstance().OnObjectHover(" ");
		}
	}

	if (mDraggable && mIsSelected) 
	{
		AddScreenPositionOffset(dx, dy);

		return true;
	}

	return false;
}

bool GUIPanel::OnMousePress(int x, int y)
{
	for (auto &&component : mComponents)
		if (component->OnMousePress(x, y))
		{
			mSelectedComponent = component;
			return true;
		}

	if (IsInside(x, y))
	{
		mIsSelected = true;

		return true;
	}

	return false;
}

bool GUIPanel::OnMouseRelease(int x, int y)
{
	mSelectedComponent = nullptr;

	for (auto &&component : mComponents)
		component->OnMouseMove(x, y, 0, 0);

	if (GUIComposite::OnMouseRelease(x, y))
		return true;

	if (mIsSelected)
		mIsSelected = false;

	return false;
}