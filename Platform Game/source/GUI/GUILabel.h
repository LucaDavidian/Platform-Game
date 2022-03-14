#ifndef GUI_LABEL_H
#define GUI_LABEL_H

#include "GUIComponent.h"
#include <string>

class Text;

class GUILabel : public GUIComponent
{
public:
	GUILabel(const glm::vec2 &position, const glm::vec2 &dimensions, const std::string &text, const std::string &fontName, unsigned int fontSize);
	~GUILabel() { delete mText; }

	void Draw() override;

	bool OnMousePress(int x, int y) override;

	void SetText(const std::string &text);
	const Text *GetText() const { return mText; }

	void SetTextColor(glm::vec3 textColor) { mTextColor = textColor; }
	glm::vec3 GetTextColor() const { return mTextColor; }

	void SetColor(glm::vec3 color) { mColor = color; }
	glm::vec3 const GetColor() const { return mColor; }

	void SetOpacity(float opacity) { mOpacity = opacity; }
	float GetOpacity() const { return mOpacity; }
private:
	Text *mText;
	glm::vec3 mColor{ 0.0f, 0.0f, 1.0f };
	glm::vec3 mTextColor{ 1.0f, 1.0f, 1.0f };
	float mOpacity;
};

#endif  // GUI_LABEL_H
