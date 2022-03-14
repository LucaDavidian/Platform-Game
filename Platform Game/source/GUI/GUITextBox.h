#ifndef GUI_TEXTBOX_H
#define GUI_TEXTBOX_H

#include "GUIComponent.h"
#include "../data structures/vector.hpp"
#include "../delegates/signal.hpp"
#include "Text.h"

class GUITextBox : public GUIComponent
{
private:
	SIGNAL_ONE_PARAM(TextSent, std::string const &);
	TextSent mTextSent;
public:
	template <typename T>
	Connection AddTextSentListener(T &listener, void(T::*ptrToMemFun)(const std::string &)) { return mTextSent.Bind(listener, ptrToMemFun); }
	template <typename T>
	Connection AddTextSentListener(T &listener, void(T::*ptrToConstMemFun)(const std::string &) const) { return mTextSent.Bind(listener, ptrToConstMemFun); }
	template <typename T>
	Connection AddTextSentListener(T &&listener) { return mTextSent.Bind(std::forward<T>(listener)); }
public:
	GUITextBox(const glm::vec2 &position, const glm::vec2 &dimensions) : GUIComponent(position, dimensions) {}

	void Draw() override;

	bool OnMousePress(int x, int y) override;
	bool OnKeyPress(unsigned int key) override;

	void InsertChar(char c);
	void InsertText(const std::string &text);

	Vector<Text*> GetTextLines() const { return mTextLines; }

	void SetColor(glm::vec3 color) { mColor = color; }
	glm::vec3 const GetColor() const { return mColor; }

	void SetOpacity(float opacity) { mOpacity = opacity; }
	float GetOpacity() const { return mOpacity; }

	unsigned GetStartLine() const { return mStart; }
private:
	Vector<Text*> mTextLines;
	unsigned int mStart = 0U;

	glm::vec3 mColor{ 0.0f, 0.0f, 0.0f };
	float mOpacity;

	std::string mFontName = "Arial.ttf";
	unsigned int mFontSize = 15U;
};

#endif  // GUI_TEXTBOX_H