#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H

#include "GUIComponent.h"
#include <string>
#include "../delegates/signal.hpp"
#include "Text.h"

class Texture;

class GUIButton : public GUIComponent
{
private:
	SIGNAL_ONE_PARAM(ButtonClicked, const std::string&);
	ButtonClicked mButtonClicked;

	SIGNAL_ONE_PARAM(ButtonHover, const std::string&);
	ButtonHover mButtonHover;
public:
	template <typename T>
	Connection AddButtonClickedListener(T &listener, void(T::*ptrToMemFun)(const std::string&)) { return mButtonClicked.Bind(listener, ptrToMemFun); }
	template <typename T>
	Connection AddButtonClickedListener(T &listener, void(T::*ptrToConstMemFun)(const std::string&) const) { return mButtonClicked.Bind(listener, ptrToConstMemFun); }
	template <typename T>
	Connection AddButtonClickedListener(T &&listener) { return mButtonClicked.Bind(std::forward<T>(listener)); }

	template <typename T>
	Connection AddButtonHoverListener(T &listener, void(T::*ptrToMemFun)(const std::string&)) { return mButtonHover.Bind(listener, ptrToMemFun); }
	template <typename T>
	Connection AddButtonHoverListener(T &listener, void(T::*ptrToConstMemFun)(const std::string&) const) { return mButtonHover.Bind(listener, ptrToConstMemFun); }
	template <typename T>
	Connection AddButtonHoverListener(T &&listener) { return mButtonHover.Bind(std::forward<T>(listener)); }
public:
	GUIButton(const glm::vec2 &position, const glm::vec2 &dimensions) : GUIComponent(position, dimensions) { SetColor(mInactiveColor); }
	~GUIButton() { delete mText;  }

	void Draw() override;

	bool OnMouseMove(int x, int y, int, int) override;
	bool OnMousePress(int x, int y) override;
	bool OnMouseRelease(int x, int y) override;

	void SetText(const std::string &text) { mText = new Text(text, "MonkeyIsland.ttf", 25); }
	const Text *GetText() const { return mText; }

	void SetColor(const glm::vec3 &color) { mColor = color; }
	glm::vec3 GetColor() const { return mColor; }
private:
	glm::vec3 mColor = glm::vec3(1.0f, 1.0f, 1.0f);
	const glm::vec3 mInactiveColor{ 0.2f, 0.2f, 0.2f };
	const glm::vec3 mActiveColor{ 1.0f, 0.0f, 0.0f };
	const glm::vec3 mSelectedColor{ 1.0f, 1.0f, 0.0f };

	Text *mText = nullptr;
};

#endif  // GUI_BUTTON_H