#ifndef POINTANDCLICKPLAYERFSM_COMPONENT_H
#define POINTANDCLICKPLAYERFSM_COMPONENT_H

#include "../Component.h"
#include "../../data structures/stack.hpp"
#include "../../data structures/vector.hpp"
#include "../../event system/Event.hpp"
#include "../../delegates/connection.hpp"
#include <glm/glm.hpp>

class PointAndClickPlayerFSMState;

class PointAndClickPlayerFSMComponent : public Component
{
	friend class FSMState;
public:
	~PointAndClickPlayerFSMComponent()
	{
		mInputConnection.Disconnect();

		for (auto &connection : mEventsConnection)
			connection.Disconnect();
	}

	void Init() override;

	void SetState(PointAndClickPlayerFSMState *newState);
	void PushState(PointAndClickPlayerFSMState *state);

	void OnInput(InputEvent inputEvent);
	void OnPlayerAtDestinationEvent(TransformAnimationEndEvent &event);
public:
	struct Input
	{
		float x, y;
	};

	static class PointAndClickPlayerStandState mStandState;
	static class PointAndClickPlayerWalkUpState mWalkUpState;
	static class PointAndClickPlayerWalkRightState mWalkRightState;
	static class PointAndClickPlayerWalkDownState mWalkDownState;
	static class PointAndClickPlayerWalkLeftState mWalkLeftState;
private:
	Stack<PointAndClickPlayerFSMState*> mStates;

	const static bool mRegistered;

	Connection mInputConnection;
	Vector<Connection> mEventsConnection;
};

class PointAndClickPlayerFSMState
{
public:
	virtual void OnEnter(PointAndClickPlayerFSMComponent *fsm) {}
	virtual void OnExit(PointAndClickPlayerFSMComponent *fsm) {}
	virtual void OnInput(PointAndClickPlayerFSMComponent *fsm, PointAndClickPlayerFSMComponent::Input input);
	virtual void OnPlayerAtDestinationEvent(PointAndClickPlayerFSMComponent *fsm, TransformAnimationEndEvent &event);
};

class PointAndClickPlayerStandState : public PointAndClickPlayerFSMState
{
public:
	void OnEnter(PointAndClickPlayerFSMComponent *fsm) override;
	void OnExit(PointAndClickPlayerFSMComponent *fsm) override;
};

class PointAndClickPlayerWalkUpState : public PointAndClickPlayerFSMState
{
public:
	void OnEnter(PointAndClickPlayerFSMComponent *fsm) override;
	void OnExit(PointAndClickPlayerFSMComponent *fsm) override;
};

class PointAndClickPlayerWalkRightState : public PointAndClickPlayerFSMState
{
public:
	void OnEnter(PointAndClickPlayerFSMComponent *fsm) override;
	void OnExit(PointAndClickPlayerFSMComponent *fsm) override;
};

class PointAndClickPlayerWalkDownState : public PointAndClickPlayerFSMState
{
public:
	void OnEnter(PointAndClickPlayerFSMComponent *fsm) override;
	void OnExit(PointAndClickPlayerFSMComponent *fsm) override;
};

class PointAndClickPlayerWalkLeftState : public PointAndClickPlayerFSMState
{
public:
	void OnEnter(PointAndClickPlayerFSMComponent *fsm) override;
	void OnExit(PointAndClickPlayerFSMComponent *fsm) override;
};

#endif // POINTANDCLICKPLAYERFSM_COMPONENT_H
