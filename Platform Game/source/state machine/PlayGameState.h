#ifndef PLAYGAMESTATE_H
#define PLAYGAMESTATE_H

#include "GameState.h"
#include "../data structures/vector.hpp"

class PlayGameState : public GameState
{
public:
	void OnEntry(Game *game) override;
	void OnExit(Game *game) override;

private:
	Vector<class Entity*> mEntities;
	class GUILifeBar *mLifeBar;
};

#endif // PLAYGAMESTATE_H
