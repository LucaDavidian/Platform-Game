#ifndef EDITORGAMESTATE_H
#define EDITORGAMESTATE_H

#include "GameState.h"
#include "../delegates/connection.hpp"
#include "../data structures/vector.hpp"
#include "../GUI/GUIComponent.h"

class EditorGameState : public GameState
{
public:
	void OnEntry(Game *game) override;
	void OnExit(Game *game) override;
private:
	void OnMouseMove(int x, int y);
	bool OnMousePress(int x, int y);
	bool OnMouseRelease(int x, int y);

	void Clear();

	void SaveArea();

	Vector<Connection> mConnections;
	Vector<GUIComponent*> mGUIComponents;
	class GUIButtonGroup *mButtonGroup;

	enum State { IDLE, PLACE_AREA, AREA_PLACED, PLACE_HOLE, REMOVE, };
	State mState = IDLE;
	State mBeforeRemove;
	unsigned mCurrentHole = 0;

	Vector<glm::vec2> mWalkArea;
	Vector<Vector<glm::vec2>> mHoles;

	StaticMesh *mWalkAreaMesh;
	Vector<StaticMesh*> mHoleMeshes;
};

#endif // EDITORGAMESTATE_H
