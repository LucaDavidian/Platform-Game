#include "GUIToggleButton.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "../ECS/systems/RenderSystem.h"
#include "../GraphicsManager.h"
#include "Text.h"

void GUIToggleButton::Draw()
{
	RenderSystem::GetInstance().SetShader(RenderSystem::GetInstance().mGUIShader);

	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(mNormalizedDimensions.x, mNormalizedDimensions.y, 0.0f));
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(GetNormalizedPositionCenter().x, GetNormalizedPositionCenter().y, 0.0f));
	glm::mat4 modelMatrix = translationMatrix * scaleMatrix;

	RenderSystem::GetInstance().GetCurrentShader().SetUniform("modelMatrix", modelMatrix);
	RenderSystem::GetInstance().GetCurrentShader().SetUniform("color", mColor.r, mColor.g, mColor.b);
	RenderSystem::GetInstance().GetCurrentShader().SetUniform("hasTexture", false);
	RenderSystem::GetInstance().GetCurrentShader().SetUniform("isOver", false);

	RenderSystem::GetInstance().GetQuadMesh().Bind();
	RenderSystem::GetInstance().GetQuadMesh().DrawTriangles();

	if (mText)
	{
		RenderSystem::GetInstance().SetShader(RenderSystem::GetInstance().mTextShader);

		mText->GetFont().GetAtlas().Bind(0);
		mText->GetMesh().Bind();

		float offX = GetNormalizedPositionCenter().x - mText->GetNormalizedDimensions().x / 2.0f - mText->GetNormalizedOffset().x;
		float offY = GetNormalizedPositionCenter().y - mText->GetNormalizedDimensions().y / 2.0f - mText->GetNormalizedOffset().y;

		glm::mat4 scaleMatrix = glm::mat4(1.f);
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(offX, offY, 0.0f));
		glm::mat4 modelMatrix = translationMatrix * scaleMatrix;

		RenderSystem::GetInstance().GetCurrentShader().SetUniform("modelMatrix", modelMatrix);

		glm::vec3 color = glm::vec3(0.1f, 1.0f, 0.2f);
		RenderSystem::GetInstance().GetCurrentShader().SetUniform("color", color.r, color.g, color.b);

		GraphicsManager::GetInstance().EnableBlending();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		mText->GetMesh().DrawTriangles();
		GraphicsManager::GetInstance().DisableBlending();
	}
}

bool GUIToggleButton::OnMouseMove(int x, int y, int, int)
{
	if (IsInside(x, y))
	{
		if (!mIsFocused)  // on mouse enter
		{
			mIsFocused = true;

			if (mToggleState == ToggleState::UNTOGGLED)
				SetColor(mIsSelected ? mSelectedColor : mActiveColor);

			mButtonHover(mText->GetText());
		}
	}
	else
	{
		if (mIsFocused)  // on mouse leave
		{
			mIsFocused = false;

			if (mToggleState == ToggleState::UNTOGGLED)
				SetColor(mInactiveColor);
		}
	}

	return false;
}

bool GUIToggleButton::OnMousePress(int x, int y)
{
	if (IsInside(x, y))
	{
		mIsSelected = true;

		SetColor(mSelectedColor);

		return true;
	}

	return false;
}

bool GUIToggleButton::OnMouseRelease(int x, int y)
{
	if (mIsSelected)
	{
		mIsSelected = false;

		if (IsInside(x, y))
		{
			if (mToggleState == ToggleState::UNTOGGLED)
			{
				SetColor(mToggledColor);
				mToggleState = ToggleState::TOGGLED;
			}
			else // mToggleState == ToggleState::TOGGLED
			{
				SetColor(mInactiveColor);
				mToggleState = ToggleState::UNTOGGLED;
			}

			mButtonToggled(mText->GetText(), mToggleState);

			return true;
		}
	}

	return false;
}