#ifndef GAMEOVERGAMESTATE_H
#define GAMEOVERGAMESTATE_H

#include "GameState.h"

class GameOverGameState : public GameState
{
public:
	void OnEntry(Game *game) override;
	void OnExit(Game *game) override;
	void OnInput(Game *game, struct InputEvent &inputEvent) override;

private:
	class GUIPanel *mGameOverPanel;
};

#endif // GAMEOVERGAMESTATE_H
