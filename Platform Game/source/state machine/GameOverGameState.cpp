#include "GameOverGameState.h"
#include "Game.h"
#include "../event system/Event.hpp"
#include "../GUI/GUIManager.h"
#include "../GUI/GUIPanel.h"
#include "../GUI/GUILabel.h"
#include "../GUI/GUIButton.h"

void GameOverGameState::OnEntry(Game *game)
{
	mGameOverPanel = new GUIPanel(glm::vec2(), glm::vec2());
	mGameOverPanel->SetAchorPoint(GUIComponent::AnchorPoint::CENTER, GUIComponent::AnchorPoint::CENTER);
	mGameOverPanel->SetRelativePosition(glm::vec2(0.0f));
	mGameOverPanel->SetRelativeDimensions(glm::vec2(1.0f, 1.0f));
	mGameOverPanel->SetOpacity(0.2f);

	GUILabel *gameOverLabel = new GUILabel(glm::vec2(), glm::vec2(), "Game Over!", "SuperLegendBoy.ttf", 45);
	gameOverLabel->SetAchorPoint(GUIComponent::AnchorPoint::CENTER, GUIComponent::AnchorPoint::CENTER);
	gameOverLabel->SetRelativePosition(glm::vec2(0.0f, 0.2f));

	GUIButton *playAgainButton = new GUIButton(glm::vec2(0.2f, 0.3f), glm::vec2(0.2f, 0.1f));
	playAgainButton->SetAchorPoint(GUIComponent::AnchorPoint::BOTTOM_LEFT, GUIComponent::AnchorPoint::BOTTOM_LEFT);
	playAgainButton->SetText("play again");

	GUIButton *quitButton = new GUIButton(glm::vec2(0.2f, 0.3f), glm::vec2(0.2f, 0.1f));
	playAgainButton->SetAchorPoint(GUIComponent::AnchorPoint::BOTTOM_RIGHT, GUIComponent::AnchorPoint::BOTTOM_RIGHT);
	quitButton->SetText("quit");

	playAgainButton->AddButtonClickedListener([game](const std::string &) 
	{ 
		game->Restart();
	});

	quitButton->AddButtonClickedListener([game](std::string const &) { game->Quit(); });

	mGameOverPanel->AddGuiComponent(gameOverLabel);
	mGameOverPanel->AddGuiComponent(playAgainButton);
	mGameOverPanel->AddGuiComponent(quitButton);

	GUIManager::GetInstance().AddGUIPanel(mGameOverPanel);
}

void GameOverGameState::OnExit(Game *game)
{
	GUIManager::GetInstance().RemoveGUIPanel(mGameOverPanel);
}

void GameOverGameState::OnInput(Game *game, InputEvent &inputEvent)
{

}
