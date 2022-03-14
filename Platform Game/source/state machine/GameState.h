#ifndef GAME_STATE_H
#define GAME_STATE_H

class Game;
class MenuGameState;
class PlayGameState;
class GameOverGameState;
struct InputEvent;
struct NoLivesLeftEvent;

class GameState
{
	friend class Game;
public:
	virtual void OnEntry(Game *game) {}
	virtual void OnExit(Game *game) {}
	virtual void OnInput(Game *game, InputEvent &inputEvent) {}   

protected:
	static MenuGameState mMenuGameState;
	static PlayGameState mPlayGameState;
	static GameOverGameState mGameOverGameState;
};

#endif // GAME_STATE_H
