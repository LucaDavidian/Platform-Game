#ifndef GAME_H
#define GAME_H

#include "../data structures/stack.hpp"
#include "../delegates/connection.hpp"

class GameState;

class Game
{
public:
	static Game &GetInstance()
	{
		static Game instance;
		return instance;
	}

	~Game();

	void Initialize();
	void Run();

	void PushState(GameState *gameState);
	void ChangeState(GameState *gameState);
	void PopState();

	void Quit() { mQuit = true; }
	void Restart();
private:
	Game();

	bool mQuit = false;
	bool mDebug = false;

	void Loop();
	void Setup();
	void ProcessInput();
	void Update(float frameTimeMs);
	void Render();

	Stack<GameState*> mGameStates;

	void OnInput(struct InputEvent &input);

	Connection mInputConnection;
	Connection mEventConnection;
};

#endif // GAME_H