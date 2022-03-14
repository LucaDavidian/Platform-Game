#ifndef GUI_BUTTONGROUP_H
#define GUI_BUTTONGROUP_H

#include "GUIPanel.h"
#include "GUIToggleButton.h"
#include "../data structures/vector.hpp"
#include "../delegates/signal.hpp"

class GUIButtonGroup : public GUIComposite
{
public:
	SIGNAL_TWO_PARAM(ButtonSelectedEvent, unsigned int, bool);
public:
	GUIButtonGroup(glm::vec2 position, glm::vec2 dimensions) : GUIComposite(position, dimensions) {}

	bool OnMouseRelease(int x, int y) override;

	void AddButton(GUIToggleButton *button);

	template <typename T>
	Connection AddButtonSelectedListener(T &instance, void (T::*ptrToMemFun)(unsigned int, bool)) { return mButtonSelectedEvent.Bind(instance, ptrToMemFun); }

	template <typename T>
	Connection AddButtonSelectedListener(T &&instance) { return mButtonSelectedEvent.Bind(std::forward<T>(instance)); }

	void ToggleButton(unsigned int button) 
	{
		assert(button < 3);

		if (mToggleButtons[button]->GetToggled() == GUIToggleButton::ToggleState::TOGGLED)
			mToggleButtons[button]->SetToggle(false);
		else
			mToggleButtons[button]->SetToggle(true);
	}
private:
	ButtonSelectedEvent mButtonSelectedEvent;

	Vector<GUIToggleButton*> mToggleButtons;
	unsigned int mSelectedButton;
};

#endif  // GUI_BUTTONGROUP_H