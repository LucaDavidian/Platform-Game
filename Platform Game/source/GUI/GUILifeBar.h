#ifndef GUI_LIFEBAR_H
#define GUI_LIFEBAR_H

#include "GUIComposite.h"
#include "../data structures/vector.hpp"
#include "../event system/EventBus.hpp"

class GUILifeBar : public GUIComposite
{
public:
	GUILifeBar(glm::vec2 position, glm::vec2 dimensions) : GUIComposite(position, dimensions), mNumLives(0U) 
	{
		GUILabel *livesLabel = new GUILabel(glm::vec2(0.0f, 0.0f), glm::vec2(mLabelWidth, 1.0f), "lives:", "SuperLegendBoy.ttf", 20);
		livesLabel->SetColor(glm::vec3(0.0f, 0.0f, 0.0f));
		livesLabel->SetOpacity(0.0f);
		AddGuiComponent(livesLabel);
		
		gEventBus.SubscribeToEvent<NumLivesChangedEvent>(*this, &GUILifeBar::OnNumLifeChanged);
	}
private:
	void OnNumLifeChanged(NumLivesChangedEvent &event)
	{
		if (event.numLives < mLives.Size())
			for (int i = event.numLives; i < mLives.Size(); i++)
				//mLives[i]->SetVisible(false);
			{
				RemoveGUIComponent(mLives.Last());
				mLives.RemoveLast();
			}
		else 
		{
			unsigned int livesToAdd = event.numLives - mLives.Size();
			unsigned currentSize = mLives.Size();

			for (int i = 0; i < livesToAdd; i++)
			{
				GUIImage *heart = new GUIImage(glm::vec2(0.4f + (currentSize + i) * mHeartWidth, 0.2f), glm::vec2(mHeartWidth, mHeartHeight), "heart.png");
				AddGuiComponent(heart);
				mLives.InsertLast(heart);
			}
		}
	}

	unsigned int mNumLives;
	Vector<GUIImage*> mLives;
	float mLabelWidth = 0.4;
	unsigned mMaxLives = 6;
	float mHeartWidth = (1.0f - mLabelWidth) / mMaxLives;  
	float mHeartHeight = 0.5f;
};

#endif // GUI_LIFEBAR_H
