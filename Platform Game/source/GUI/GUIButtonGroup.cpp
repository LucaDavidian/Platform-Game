#include "GUIButtonGroup.h"

bool GUIButtonGroup::OnMouseRelease(int x, int y)
{
	for (size_t i = 0; i < mToggleButtons.Size(); i++)
	{
		if (mToggleButtons[i]->OnMouseRelease(x, y))
		{
			if (mToggleButtons[i]->GetToggled() == GUIToggleButton::ToggleState::TOGGLED)  // button is toggled: untoggle other
			{
				for (auto &toggleButton : mToggleButtons)
					if (toggleButton != mToggleButtons[i])
						toggleButton->SetToggle(false);

				mButtonSelectedEvent(i, true);
			}
			else  // button is not toggled
				mButtonSelectedEvent(i, false);

			return true;
		}
	}

	return false;
}

void GUIButtonGroup::AddButton(GUIToggleButton *button)
{
	mToggleButtons.InsertLast(button);

	int i = 0;
	for (auto &toggleButton : mToggleButtons)
	{
		toggleButton->SetRelativePosition(glm::vec2(i * 1.0f / mToggleButtons.Size(), 0.0f));
		toggleButton->SetRelativeDimensions(glm::vec2(1.0f / mToggleButtons.Size(), 1.0f));
		i++;
	}

	AddGuiComponent(button);
}
