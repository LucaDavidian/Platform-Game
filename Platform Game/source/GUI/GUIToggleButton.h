#ifndef GUI_TOGGLEBUTTON_H
#define GUI_TOGGLEBUTTON_H

#include "GUIComponent.h"
#include "Text.h"
#include "../delegates/signal.hpp"
#include <string>

class GUIToggleButton : public GUIComponent
{
public:
	enum class ToggleState { UNTOGGLED, TOGGLED };
private:
	SIGNAL_ONE_PARAM(ButtonHover, const std::string&);
	ButtonHover mButtonHover;

	SIGNAL_TWO_PARAM(ButtonToggled, const std::string&, ToggleState);
	ButtonToggled mButtonToggled;
public:
	template <typename T>
	Connection AddButtonToggledListener(T &listener, void(T::*ptrToMemFun)(const std::string&)) { return mButtonToggled.Bind(listener, ptrToMemFun); }
	template <typename T>
	Connection AddButtonToggledListener(T &listener, void(T::*ptrToConstMemFun)(const std::string&) const) { return mButtonToggled.Bind(listener, ptrToConstMemFun); }
	template <typename T>
	Connection AddButtonToggledListener(T &&listener) { return mButtonToggled.Bind(std::forward<T>(listener)); }

	template <typename T>
	Connection AddButtonHoverListener(T &listener, void(T::*ptrToMemFun)(const std::string&)) { return mButtonHover.Bind(listener, ptrToMemFun); }
	template <typename T>
	Connection AddButtonHoverListener(T &listener, void(T::*ptrToConstMemFun)(const std::string&) const) { return mButtonHover.Bind(listener, ptrToConstMemFun); }
	template <typename T>
	Connection AddButtonHoverListener(T &&listener) { return mButtonHover.Bind(std::forward<T>(listener)); }
public:
	GUIToggleButton(const glm::vec2 &position, const glm::vec2 &dimensions) : GUIComponent(position, dimensions) { SetColor(mInactiveColor); }
	~GUIToggleButton() { delete mText; }

	void Draw() override;

	bool OnMouseMove(int x, int y, int, int) override;
	bool OnMousePress(int x, int y) override;
	bool OnMouseRelease(int x, int y) override;

	void SetText(const std::string &text) { mText = new Text(text, "MonkeyIsland.ttf", 25); }
	const Text *GetText() const { return mText; }

	void SetColor(const glm::vec3 &color) { mColor = color; }
	glm::vec3 GetColor() const { return mColor; }

	void SetToggle(bool toggle)
	{
		if (toggle)
		{
			mToggleState = ToggleState::TOGGLED;
			mColor = mToggledColor;
		}
		else
		{
			mToggleState = ToggleState::UNTOGGLED;
			mColor = mInactiveColor;
		}
	}

	ToggleState GetToggled() const { return mToggleState; }
private:
	glm::vec3 mColor = glm::vec3(1.0f, 1.0f, 1.0f);
	const glm::vec3 mInactiveColor{ 0.2f, 0.2f, 0.2f };
	const glm::vec3 mActiveColor{ 1.0f, 0.0f, 0.0f };
	const glm::vec3 mSelectedColor{ 1.0f, 1.0f, 0.0f };
	const glm::vec3 mToggledColor{ 0.0f, 0.0f, 1.0f };

	Text *mText = nullptr;

	ToggleState mToggleState = ToggleState::UNTOGGLED;
};

#endif // GUI_TOGGLEBUTTON_H
