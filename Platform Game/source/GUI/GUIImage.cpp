#include "GUIImage.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "../ECS/systems/RenderSystem.h"
#include "../GraphicsManager.h"
#include "../asset manager/TextureManager.h"

GUIImage::GUIImage(const glm::vec2 &position, const glm::vec2 &dimensions, const std::string &image) 
	: GUIComponent(position, dimensions) 
{
	if (image.empty())
		mTexture = nullptr;
	else
	{
		mTexture = &TextureManager::GetInstance().GetTexture(image);
		mImage = image;
	}
}

void GUIImage::Draw()
{
	if (!mIsVisible)
		return;

	RenderSystem::GetInstance().SetShader(RenderSystem::GetInstance().mGUIShader);

	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(mNormalizedDimensions.x, mNormalizedDimensions.y, 0.0f));
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(GetNormalizedPositionCenter().x, GetNormalizedPositionCenter().y, 0.0f));
	glm::mat4 modelMatrix = translationMatrix * scaleMatrix;

	RenderSystem::GetInstance().GetCurrentShader().SetUniform("modelMatrix", modelMatrix);

	if (mTexture)
	{
		mTexture->Bind(0);
		RenderSystem::GetInstance().GetCurrentShader().SetUniform("hasTexture", true);
	}
	else
	{
		RenderSystem::GetInstance().GetCurrentShader().SetUniform("color", mColor.r, mColor.g, mColor.b);
		RenderSystem::GetInstance().GetCurrentShader().SetUniform("hasTexture", false);
	}

	RenderSystem::GetInstance().GetCurrentShader().SetUniform("isOver", mIsOver);

	GraphicsManager::GetInstance().EnableBlending();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	RenderSystem::GetInstance().GetQuadMesh().Bind();
	RenderSystem::GetInstance().GetQuadMesh().DrawTriangles();
	GraphicsManager::GetInstance().DisableBlending();
}

void GUIImage::SetImage(const std::string &image)
{
	mTexture = &TextureManager::GetInstance().GetTexture(image + ".gif");
	mImage = image;
}

void GUIImage::RemoveImage()
{
	mTexture = nullptr;
	mImage = " ";
}

bool GUIImage::OnMouseMove(int x, int y, int dx, int dy)
{
	if (IsInside(x, y))
	{
		if (!mIsFocused)
		{
			mIsFocused = true;

			if (mImage != " ")
				mIsOver = true;

			mImageHover(mImage);
		}
	}
	else
	{
		if (mIsFocused)
		{
			mIsFocused = false;

			mIsOver = false;	
		}
	}

	return false;
}

bool GUIImage::OnMousePress(int x, int y)
{
	if (IsInside(x, y))
	{
		mImageClicked(mImage);
	}

	return false;
}

