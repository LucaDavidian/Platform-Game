#include "EnemyFSMComponent.h"
#include "SpriteAnimationComponent.h"
#include "SpriteComponent.h"
#include "PhysicsComponent.h"
#include "../../Input/InputManager.h"
#include "../../Lua bindings/LuaVM.h"
#include "../Entity.h"
#include "../systems/AudioSystem.h"

HoverState EnemyFSMComponent::mHoverState;
FlyState EnemyFSMComponent::mFlyState;
EnemyAttackState EnemyFSMComponent::mAttackState;
DyingState EnemyFSMComponent::mDyingState;
DeadState EnemyFSMComponent::mDeadState;

namespace
{
	EnemyFSMComponent EnemyFSMComponentTableCtor(LuaObject componentTable)
	{
		return EnemyFSMComponent();
	}

	void EnemyFSMComponentAddToEntity(EnemyFSMComponent const &fsmComponent, Entity &entity)
	{
		entity.AddComponent<EnemyFSMComponent>(fsmComponent);
	}
}

const bool EnemyFSMComponent::mRegistered = []() {
	Reflect::Reflect<EnemyFSMComponent>("enemy_fsm")
		.AddConstructor(EnemyFSMComponentTableCtor)
		.AddMemberFunction(EnemyFSMComponentAddToEntity, "add_to_entity");

	return true;
}();

void EnemyFSMComponent::Init()
{
	// subscribe to events
	mEventsConnection.InsertLast(GetOwner()->GetEventBus().SubscribeToEvent<SpriteAnimationEndEvent>(*this, &EnemyFSMComponent::OnAnimationEndEvent));
	mEventsConnection.InsertLast(GetOwner()->GetEventBus().SubscribeToEvent<EnemyDeadEvent>(*this, &EnemyFSMComponent::OnEnemyDeadEvent));

	// initial transition
	PushState(&mHoverState);
}

void EnemyFSMComponent::SetState(EnemyFSMState *newState)
{
	mStates.Top()->OnExit(this);
	mStates.Pop();
	mStates.Push(newState);
	mStates.Top()->OnEnter(this);
}

void EnemyFSMComponent::PushState(EnemyFSMState *state)
{
	mStates.Push(state);
	mStates.Top()->OnEnter(this);
}

void EnemyFSMComponent::OnInput(Input input)
{
	mStates.Top()->OnInput(this, input);
}

void EnemyFSMComponent::OnAnimationEndEvent(SpriteAnimationEndEvent &event)
{
	mStates.Top()->OnAnimationEndEvent(this, event);
}

void EnemyFSMComponent::OnEnemyDeadEvent(EnemyDeadEvent &event)
{
	mStates.Top()->OnEnemyDeadEvent(this, event);
}

/***** enemy states *****/

/* hover state */
void HoverState::OnEnter(EnemyFSMComponent *fsm)
{
	if (fsm->GetOwner()->HasComponent<SpriteAnimationComponent>())
		fsm->GetOwner()->GetComponent<SpriteAnimationComponent>()->PlayAnimation("flight", true);
}

void HoverState::OnInput(EnemyFSMComponent *fsm, EnemyFSMComponent::Input input)
{
	if (input == EnemyFSMComponent::Input::FLY_RIGHT || input == EnemyFSMComponent::Input::FLY_LEFT)
		fsm->SetState(&EnemyFSMComponent::mFlyState);

	if (input == EnemyFSMComponent::Input::ATTACK)
		fsm->SetState(&EnemyFSMComponent::mAttackState);
}

void HoverState::OnEnemyDeadEvent(EnemyFSMComponent *fsm, EnemyDeadEvent &event)
{
	fsm->SetState(&EnemyFSMComponent::mDyingState);
}

/* fly state */
void FlyState::OnEnter(EnemyFSMComponent *fsm)
{
	if (fsm->GetOwner()->HasComponent<SpriteAnimationComponent>())
		fsm->GetOwner()->GetComponent<SpriteAnimationComponent>()->PlayAnimation("flight", true);
}

void FlyState::OnInput(EnemyFSMComponent *fsm, EnemyFSMComponent::Input input)
{
	if (input == EnemyFSMComponent::Input::FLY_RIGHT)
	{
		if (fsm->GetOwner()->HasComponents<PhysicsComponent>())
		{
			auto physicsComponent = fsm->GetOwner()->GetComponent<PhysicsComponent>();
			physicsComponent->SetVelocity(glm::vec2(1.0f * mFlySpeed, physicsComponent->GetVelocity().y));
		}

		if (fsm->GetOwner()->HasComponents<SpriteComponent>())
			fsm->GetOwner()->GetComponent<SpriteComponent>()->Flip(false);
	}

	if (input == EnemyFSMComponent::Input::FLY_LEFT)
	{
		if (fsm->GetOwner()->HasComponents<PhysicsComponent>())
		{
			auto physicsComponent = fsm->GetOwner()->GetComponent<PhysicsComponent>();
			physicsComponent->SetVelocity(glm::vec2(-1.0f * mFlySpeed, physicsComponent->GetVelocity().y));
		}

		if (fsm->GetOwner()->HasComponents<SpriteComponent>())
			fsm->GetOwner()->GetComponent<SpriteComponent>()->Flip(true);
	}
}

void FlyState::OnEnemyDeadEvent(EnemyFSMComponent *fsm, EnemyDeadEvent &event)
{
	fsm->SetState(&EnemyFSMComponent::mDyingState);
}

/* attack state */
void EnemyAttackState::OnEnter(EnemyFSMComponent *fsm)
{
	if (fsm->GetOwner()->HasComponent<SpriteAnimationComponent>())
		fsm->GetOwner()->GetComponent<SpriteAnimationComponent>()->PlayAnimation("attck");
}

void EnemyAttackState::OnInput(EnemyFSMComponent *fsm, EnemyFSMComponent::Input input)
{

}

void EnemyAttackState::OnAnimationEndEvent(EnemyFSMComponent *fsm, SpriteAnimationEndEvent &event)
{

}

void EnemyAttackState::OnEnemyDeadEvent(EnemyFSMComponent *fsm, EnemyDeadEvent &event)
{
	fsm->SetState(&EnemyFSMComponent::mDyingState);
}

/* dying state */
void DyingState::OnEnter(EnemyFSMComponent *fsm)
{
	if (fsm->GetOwner()->HasComponent<SpriteAnimationComponent>())
		fsm->GetOwner()->GetComponent<SpriteAnimationComponent>()->PlayAnimation("death");

	auto physicsComponent = fsm->GetOwner()->AddComponent<PhysicsComponent>();
	physicsComponent->SetMass(2.0f);
}

void DyingState::OnAnimationEndEvent(EnemyFSMComponent *fsm, SpriteAnimationEndEvent &event)
{
	fsm->SetState(&EnemyFSMComponent::mDeadState);
}

/* dead state */
void DeadState::OnEnter(EnemyFSMComponent *fsm)
{
	// #TODO_play_particle_effect

	fsm->GetOwner()->Destroy();
}
