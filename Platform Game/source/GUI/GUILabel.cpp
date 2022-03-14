#include "GUILabel.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "../ECS/systems/RenderSystem.h"
#include "../GraphicsManager.h"
#include "Text.h"

GUILabel::GUILabel(const glm::vec2 &position, const glm::vec2 &dimensions, const std::string &text, const std::string &fontName, unsigned int fontSize) 
	: GUIComponent(position, dimensions), mText(new Text(text, fontName, fontSize)) 
{
}

void GUILabel::Draw()
{
	RenderSystem::GetInstance().SetShader(RenderSystem::GetInstance().mGUIShader);

	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(mNormalizedDimensions.x, mNormalizedDimensions.y, 0.0f));
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(GetNormalizedPositionCenter().x, GetNormalizedPositionCenter().y, 0.0f));
	glm::mat4 modelMatrix = translationMatrix * scaleMatrix;

	RenderSystem::GetInstance().GetCurrentShader().SetUniform("hasTexture", false);
	RenderSystem::GetInstance().GetCurrentShader().SetUniform("modelMatrix", modelMatrix);
	RenderSystem::GetInstance().GetCurrentShader().SetUniform("color", mColor.r, mColor.g, mColor.b); 
	RenderSystem::GetInstance().GetCurrentShader().SetUniform("opacity", mOpacity);

	GraphicsManager::GetInstance().EnableBlending();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	RenderSystem::GetInstance().GetQuadMesh().Bind();
	RenderSystem::GetInstance().GetQuadMesh().DrawTriangles();
	GraphicsManager::GetInstance().DisableBlending();

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
		RenderSystem::GetInstance().GetCurrentShader().SetUniform("color", mTextColor.r, mTextColor.g, mTextColor.b);

		GraphicsManager::GetInstance().EnableBlending();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		mText->GetMesh().DrawTriangles();
		GraphicsManager::GetInstance().DisableBlending();
	}
}

bool GUILabel::OnMousePress(int x, int y)
{
	if (IsInside(x, y))
		return true;
}

void GUILabel::SetText(const std::string &text)
{
	mText->Set(text);

	// delete mText
	//mText = new Text(text, mFontName, mFontSize);
}