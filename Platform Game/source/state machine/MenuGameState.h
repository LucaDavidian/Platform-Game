#ifndef MENUGAMESTATE_H
#define MENUGAMESTATE_H

#include "GameState.h"
#include "../data structures/vector.hpp"

class Entity;
class GUIComponent;

class MenuGameState : public GameState
{
public:
	void OnEntry(Game *game) override;
	void OnExit(Game *game) override;
	void OnInput(Game *game, InputEvent &inputEvent) override;

private:
	Vector<Entity*> mEntities;
	Vector<GUIComponent*> mGUIComponents;
};

#endif // MENUGAMESTATE_H
