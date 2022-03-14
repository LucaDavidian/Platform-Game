#include "GUISlider.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "../ECS/systems/RenderSystem.h"

void GUISlider::Draw()
{
	RenderSystem::GetInstance().SetShader(RenderSystem::GetInstance().mGUIShader);

	// draw slider
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(mNormalizedDimensions.x, mNormalizedDimensions.y, 0.0f));
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(GetNormalizedPositionCenter().x, GetNormalizedPositionCenter().y, 0.0f));
	glm::mat4 modelMatrix = translationMatrix * scaleMatrix;

	RenderSystem::GetInstance().GetCurrentShader().SetUniform("modelMatrix", modelMatrix);
	RenderSystem::GetInstance().GetCurrentShader().SetUniform("color", mColor.r, mColor.g, mColor.b);
	RenderSystem::GetInstance().GetCurrentShader().SetUniform("hasTexture", false);
	RenderSystem::GetInstance().GetCurrentShader().SetUniform("isOver", false);

	RenderSystem::GetInstance().GetQuadMesh().Bind();
	RenderSystem::GetInstance().GetQuadMesh().DrawTriangles();

	// draw cursor
	{
		glm::vec2 cursorPosition;
		glm::vec2 cursorDimensions;

		if (mOrientation == GUISlider::HORIZONTAL)
		{
			cursorPosition.x = GetNormalizedPositionCenter().x + (mCursorPosition - 0.5f) * (mNormalizedDimensions.x - mCursorDimension * mNormalizedDimensions.x - mNormalizedDimensions.y * 0.2);
			cursorPosition.y = GetNormalizedPositionCenter().y;

			cursorDimensions.x = mNormalizedDimensions.x * mCursorDimension;
			cursorDimensions.y = mNormalizedDimensions.y * 0.8f;
		}
		else  // GUISlider::VERTICAL
		{
			cursorPosition.x = GetNormalizedPositionCenter().x;
			cursorPosition.y = GetNormalizedPositionCenter().y - (mCursorPosition - 0.5f) * (mNormalizedDimensions.y - mCursorDimension * mNormalizedDimensions.y - mNormalizedDimensions.x * 0.2);

			cursorDimensions.x = mNormalizedDimensions.x * 0.8f;
			cursorDimensions.y = mNormalizedDimensions.y * mCursorDimension;
		}

		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), glm::vec3(cursorDimensions.x, cursorDimensions.y, 0.0f));
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0), glm::vec3(cursorPosition.x, cursorPosition.y, 0.0f));
		glm::mat4 modelMatrix = translationMatrix * scaleMatrix;

		RenderSystem::GetInstance().GetCurrentShader().SetUniform("modelMatrix", modelMatrix);
		RenderSystem::GetInstance().GetCurrentShader().SetUniform("color", mCursorColor.r, mCursorColor.g, mCursorColor.b);
		RenderSystem::GetInstance().GetCurrentShader().SetUniform("isOver", mIsOver);

		RenderSystem::GetInstance().GetQuadMesh().Bind();
		RenderSystem::GetInstance().GetQuadMesh().DrawTriangles();
	}
}

bool GUISlider::OnMouseMove(int x, int y, int dx, int dy)
{
	if (IsInside(x, y))
	{
		if (!mIsFocused)
			mIsFocused = true;
	}
	else
	{
		if (mIsFocused)
			mIsFocused = false;
	}

	if (mIsSelected)
	{
		float oldCursorPosition = mCursorPosition;

		switch (mOrientation)
		{
			case HORIZONTAL:
				mCursorPosition += (float)dx / mScreenDimensions.x;
				if (mCursorPosition >= 1.0f)
					mCursorPosition = 1.0f;
				if (mCursorPosition <= 0.0f)
					mCursorPosition = 0.0f;

				break;
			case VERTICAL:
				mCursorPosition -= (float)dy / mScreenDimensions.y;
				if (mCursorPosition >= 1.0f)
					mCursorPosition = 1.0f;
				if (mCursorPosition <= 0.0f)
					mCursorPosition = 0.0f;
				break;
		}

		mCursorMoveEvent(mCursorPosition - oldCursorPosition);

		return true;
	}

	return false;
}

bool GUISlider::OnMousePress(int x, int y)
{
	if (IsInside(x, y))
	{
		switch (mOrientation)
		{
			case HORIZONTAL:
				if (x >= GetScreenPositionCenter().x + (mCursorPosition - 0.5f) * (mScreenDimensions.x - mCursorDimension * mScreenDimensions.x) - mCursorDimension * mScreenDimensions.x / 2.0f &&
					x <= GetScreenPositionCenter().x + (mCursorPosition - 0.5f) * (mScreenDimensions.x - mCursorDimension * mScreenDimensions.x) + mCursorDimension * mScreenDimensions.x / 2.0f)
					mIsSelected = true;

				return true;

				break;
			case VERTICAL:

				if (y <= GetScreenPositionCenter().y - (mCursorPosition - 0.5f) * (mScreenDimensions.y - mCursorDimension * mScreenDimensions.y) + mCursorDimension * mScreenDimensions.y / 2.0f &&
					y >= GetScreenPositionCenter().y - (mCursorPosition - 0.5f) * (mScreenDimensions.y - mCursorDimension * mScreenDimensions.y) - mCursorDimension * mScreenDimensions.y / 2.0f)
					mIsSelected = true;

				return true;

				break;
		}
	}

	return false;
}

bool GUISlider::OnMouseRelease(int x, int y)
{
	if (mIsSelected)
		mIsSelected = false;

	return false;
}