#ifndef ENEMYFSM_COMPONENT_H
#define ENEMYFSM_COMPONENT_H

#include "../Component.h"
#include "../../data structures/stack.hpp"
#include "../../data structures/vector.hpp"
#include "../../event system/Event.hpp"
#include "../../delegates/connection.hpp"
#include <glm/glm.hpp>

class EnemyFSMState;

class EnemyFSMComponent : public Component
{
	friend class FSMState;
public:
	enum class Input
	{
		FLY_RIGHT, FLY_LEFT, HOVER, ATTACK,
	};
public:
	~EnemyFSMComponent()
	{
		for (auto &connection : mEventsConnection)
			connection.Disconnect();
	}

	void Init() override;

	void SetState(EnemyFSMState *newState);
	void PushState(EnemyFSMState *state);

	void OnInput(Input input);
	void OnAnimationEndEvent(SpriteAnimationEndEvent &event);
	void OnEnemyDeadEvent(EnemyDeadEvent &event);
public:
	static class HoverState mHoverState;
	static class FlyState mFlyState;
	static class EnemyAttackState mAttackState;
	static class DyingState mDyingState;
	static class DeadState mDeadState;
private:
	Stack<EnemyFSMState*> mStates;

	const static bool mRegistered;

	Vector<Connection> mEventsConnection;
};

class EnemyFSMState
{
public:
	virtual void OnEnter(EnemyFSMComponent *fsm) {}
	virtual void OnExit(EnemyFSMComponent *fsm) {}
	virtual void OnInput(EnemyFSMComponent *fsm, EnemyFSMComponent::Input input) {}
	virtual void OnAnimationEndEvent(EnemyFSMComponent *fsm, SpriteAnimationEndEvent &event) {}
	virtual void OnEnemyDeadEvent(EnemyFSMComponent *fsm, EnemyDeadEvent &event) {}
};

class HoverState : public EnemyFSMState
{
public:
	void OnEnter(EnemyFSMComponent *fsm) override;
	void OnInput(EnemyFSMComponent *fsm, EnemyFSMComponent::Input input) override;
	void OnEnemyDeadEvent(EnemyFSMComponent *fsm, EnemyDeadEvent &event) override;
};

class FlyState : public EnemyFSMState
{
public:
	void OnEnter(EnemyFSMComponent *fsm) override;
	void OnInput(EnemyFSMComponent *fsm, EnemyFSMComponent::Input input) override;
	void OnEnemyDeadEvent(EnemyFSMComponent *fsm, EnemyDeadEvent &event) override;
private:
	float mFlySpeed = 1.0f;
};

class EnemyAttackState : public EnemyFSMState
{
public:
	void OnEnter(EnemyFSMComponent *fsm) override;
	void OnInput(EnemyFSMComponent *fsm, EnemyFSMComponent::Input input) override;
	void OnAnimationEndEvent(EnemyFSMComponent *fsm, SpriteAnimationEndEvent &event) override;
	void OnEnemyDeadEvent(EnemyFSMComponent *fsm, EnemyDeadEvent &event) override;
};

class DyingState : public EnemyFSMState
{
public:
	void OnEnter(EnemyFSMComponent *fsm) override;
	void OnAnimationEndEvent(EnemyFSMComponent *fsm, SpriteAnimationEndEvent &event) override;
};

class DeadState : public EnemyFSMState
{
public:
	void OnEnter(EnemyFSMComponent *fsm) override;
};

#endif // ENEMYFSM_COMPONENT_H
