#include "GUITextBox.h"
#include <SDL2/SDL.h>

void GUITextBox::Draw()
{
	//SetShader(mGUIShader);
	//
	//			glm::vec2 textBoxPosition = textBox->GetNormalizedPositionCenter();
	//			glm::vec2 textBoxDimensions = textBox->GetNormalizedDimensions();
	//
	//			// draw text box
	//			glm::mat4 scaleMatrix = glm::mat4(1.0f);
	//			glm::mat4 translationMatrix = glm::mat4(1.0f);
	//			scaleMatrix = glm::scale(scaleMatrix, glm::vec3(textBoxDimensions.x, textBoxDimensions.y, 0.0f));
	//			translationMatrix = glm::translate(translationMatrix, glm::vec3(textBoxPosition.x, textBoxPosition.y, 0.0f));
	//			glm::mat4 modelMatrix = translationMatrix * scaleMatrix;
	//
	//			mCurrentShader->SetUniform("modelMatrix", modelMatrix);
	//			glm::vec3 color = textBox->GetColor();
	//			mCurrentShader->SetUniform("color", color.r, color.g, color.b);  // currently bound shader program - TODO: uniform not found
	//
	//			mQuadMesh.Bind();
	//			mQuadMesh.DrawTriangles();
	//
	//			// draw text
	//
	//			SetShader(mTextShader);
	//
	//			unsigned int line = 0U;
	//			for (auto text : textBox->GetTextLines())
	//			{
	//				if (line < textBox->GetStartLine())
	//				{
	//					line++;
	//					continue;
	//				}
	//
	//				textBoxPosition = textBox->GetNormalizedPositionTL();
	//
	//				float screenHeightSizePerPixel = 2.0f / GraphicsSystem::GetInstance().GetViewportHeight();  // TODO: ugly
	//				float lineSpacingNorm = text->GetFont().GetLineSpacing() * -screenHeightSizePerPixel;
	//
	//				float offX = textBoxPosition.x - text->GetNormalizedOffset().x;
	//				float offY = lineSpacingNorm * (line - textBox->GetStartLine()) + textBoxPosition.y + lineSpacingNorm;
	//
	//				scaleMatrix = glm::mat4(1.f);
	//				translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(offX, offY, 0.0f));
	//				modelMatrix = translationMatrix * scaleMatrix;
	//
	//				mCurrentShader->SetUniform("modelMatrix", modelMatrix);
	//				mCurrentShader->SetUniform("color", 0.0f, 1.0f, 0.4f);
	//
	//				text->GetFont().GetAtlas().Bind(0);
	//
	//				GraphicsSystem::GetInstance().EnableBlending();
	//				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//				text->GetMesh().Bind();
	//				text->GetMesh().DrawTriangles();
	//				GraphicsSystem::GetInstance().DisableBlending();
	//
	//				line++;
}

bool GUITextBox::OnMousePress(int x, int y)
{
	return false;
}

bool GUITextBox::OnKeyPress(unsigned int key)
{
	if (key == SDL_SCANCODE_RETURN)
	{
		mTextSent(mTextLines.Last()->GetText());
		InsertChar('\n');
	}
	else
		InsertChar(SDL_GetKeyFromScancode((SDL_Scancode)key));

	return false;
}

#include <cctype>

void GUITextBox::InsertChar(char c)
{
	if (mTextLines.Empty())
		mTextLines.InsertLast(new Text(mFontName, mFontSize, 100));

	if (c == '\n')
		mTextLines.InsertLast(new Text(mFontName, mFontSize, 100));
	else if (Text::GetTextBoundingBox(mTextLines.Last()->GetText() + c, mFontName, mFontSize).x >= mNormalizedDimensions.x)
	{
		mTextLines.InsertLast(new Text(mFontName, mFontSize, 100));
		mTextLines.Last()->Append(c);
	}
	else
		mTextLines.Last()->Append(c);

	if (mTextLines[0]->GetFont().GetLineSpacing() * (mTextLines.Size() - mStart) >= mScreenDimensions.y)
		mStart++;
}

void GUITextBox::InsertText(const std::string &text)
{
	/*if (mTextLines.Empty())
		mTextLines.InsertLast(new Text(mFontName, mFontSize, 100));

	if (Text::GetSize(mTextLines.Last()->GetText() + text, mFontName, mFontSize).x >= mNormalizedDimensions.x)
	{
		mTextLines.InsertLast(new Text(mFontName, mFontSize, 100));
		mTextLines.Last()->Append(text);
	}
	else
		mTextLines.Last()->Append(text);*/

	for (auto c : text)
		InsertChar(c);
}