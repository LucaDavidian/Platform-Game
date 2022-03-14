#include "PointAndClickPlayerFSMComponent.h"
#include "../../Lua bindings/LuaVM.h"
#include "../Entity.h"
#include "../../Input/InputManager.h"
#include "SpriteComponent.h"
#include "SpriteAnimationComponent.h"
#include "TransformComponent.h"
#include "TransformAnimationComponent.h"
#include "CameraComponent.h"
#include "../Scene.h"

PointAndClickPlayerStandState PointAndClickPlayerFSMComponent::mStandState;
PointAndClickPlayerWalkUpState PointAndClickPlayerFSMComponent::mWalkUpState;
PointAndClickPlayerWalkRightState PointAndClickPlayerFSMComponent::mWalkRightState;
PointAndClickPlayerWalkDownState PointAndClickPlayerFSMComponent::mWalkDownState;
PointAndClickPlayerWalkLeftState PointAndClickPlayerFSMComponent::mWalkLeftState;

namespace
{
	PointAndClickPlayerFSMComponent PointAndClickPlayerFSMComponentTableCtor(LuaObject componentTable)
	{
		return PointAndClickPlayerFSMComponent();
	}

	void PointAndClickPlayerFSMComponentAddToEntity(PointAndClickPlayerFSMComponent const &fsmComponent, Entity &entity)
	{
		entity.AddComponent<PointAndClickPlayerFSMComponent>(fsmComponent);
	}
}

const bool PointAndClickPlayerFSMComponent::mRegistered = []() {
	Reflect::Reflect<PointAndClickPlayerFSMComponent>("point_and_click_player_fsm")
		.AddConstructor(PointAndClickPlayerFSMComponentTableCtor)
		.AddMemberFunction(PointAndClickPlayerFSMComponentAddToEntity, "add_to_entity");

	return true;
}();

void PointAndClickPlayerFSMComponent::Init()
{
	// subscribe to input and events
	mInputConnection = InputManager::GetInstance().RegisterInputEvent(*this, &PointAndClickPlayerFSMComponent::OnInput);
	mEventsConnection.InsertLast(GetOwner()->GetEventBus().SubscribeToEvent<TransformAnimationEndEvent>(*this, &PointAndClickPlayerFSMComponent::OnPlayerAtDestinationEvent));

	// initial transition
	PushState(&mStandState);
}

void PointAndClickPlayerFSMComponent::SetState(PointAndClickPlayerFSMState *newState)
{
	mStates.Top()->OnExit(this);
	mStates.Pop();
	mStates.Push(newState);
	mStates.Top()->OnEnter(this);
}

void PointAndClickPlayerFSMComponent::PushState(PointAndClickPlayerFSMState *state)
{
	mStates.Push(state);
	mStates.Top()->OnEnter(this);
}

void PointAndClickPlayerFSMComponent::OnInput(InputEvent inputEvent)   // translate raw, low-level input to gameplay actions
{
	switch (inputEvent.mType)
	{
		case InputEvent::Type::MOUSE_PRESS:

			Input input{ inputEvent.mousePressEvent.x, inputEvent.mousePressEvent.y };
			mStates.Top()->OnInput(this, input);

			break;
	}
}

void PointAndClickPlayerFSMComponent::OnPlayerAtDestinationEvent(TransformAnimationEndEvent &event)
{
	mStates.Top()->OnPlayerAtDestinationEvent(this, event);
}

// player states

void PointAndClickPlayerFSMState::OnInput(PointAndClickPlayerFSMComponent *fsm, PointAndClickPlayerFSMComponent::Input input)
{
	glm::vec2 destination = Scene::GetInstance().GetCamera()->GetComponent<CameraComponent>()->GetPointOnNearPlane(input.x, input.y);
	glm::vec2 direction = glm::vec3(destination, 0.0f) - fsm->GetOwner()->GetComponent<TransformComponent>()->GetPosition();

	float distance = glm::length(direction);
	direction = glm::normalize(direction);

	if (direction.x >= 0)
		if (direction.x >= glm::abs(direction.y))
			fsm->SetState(&PointAndClickPlayerFSMComponent::mWalkRightState);
		else if (direction.y >= 0)
			fsm->SetState(&PointAndClickPlayerFSMComponent::mWalkUpState);
		else
			fsm->SetState(&PointAndClickPlayerFSMComponent::mWalkDownState);
	else
		if (-direction.x >= glm::abs(direction.y))
			fsm->SetState(&PointAndClickPlayerFSMComponent::mWalkLeftState);
		else if (direction.y >= 0)
			fsm->SetState(&PointAndClickPlayerFSMComponent::mWalkUpState);
		else
			fsm->SetState(&PointAndClickPlayerFSMComponent::mWalkDownState);

	if (fsm->GetOwner()->HasComponents<TransformComponent, TransformAnimationComponent>())
	{
		auto transformComponent = fsm->GetOwner()->GetComponent<TransformComponent>();

		fsm->GetOwner()->GetComponent<TransformAnimationComponent>()->PlayAnimation(
			Vector<TransformAnimationComponent::Animation::KeyFrame>{ {transformComponent->GetPosition(), transformComponent->GetOrientation(), transformComponent->GetScale(), 0.0f}, { glm::vec3(destination, 0.0f), transformComponent->GetOrientation(), transformComponent->GetScale(), distance } }
		);
	}
}

void PointAndClickPlayerFSMState::OnPlayerAtDestinationEvent(PointAndClickPlayerFSMComponent *fsm, TransformAnimationEndEvent &event)
{
	fsm->SetState(&PointAndClickPlayerFSMComponent::mStandState);
}

// stand state
void PointAndClickPlayerStandState::OnEnter(PointAndClickPlayerFSMComponent *fsm)
{
	if (fsm->GetOwner()->HasComponent<SpriteAnimationComponent>())
		fsm->GetOwner()->GetComponent<SpriteAnimationComponent>()->PlayAnimation("stand", true);  // #TODO_decouple
}

void PointAndClickPlayerStandState::OnExit(PointAndClickPlayerFSMComponent *fsm)
{

}

// walk up state
void PointAndClickPlayerWalkUpState::OnEnter(PointAndClickPlayerFSMComponent *fsm)
{
	if (fsm->GetOwner()->HasComponents<SpriteAnimationComponent, SpriteComponent>())
		fsm->GetOwner()->GetComponent<SpriteAnimationComponent>()->PlayAnimation("walk_up", true);  // #TODO_decouple
}

void PointAndClickPlayerWalkUpState::OnExit(PointAndClickPlayerFSMComponent *fsm)
{

}

// walk right state
void PointAndClickPlayerWalkRightState::OnEnter(PointAndClickPlayerFSMComponent *fsm)
{
	if (fsm->GetOwner()->HasComponents<SpriteAnimationComponent, SpriteComponent>())
		fsm->GetOwner()->GetComponent<SpriteAnimationComponent>()->PlayAnimation("walk_right", true);  // #TODO_decouple
}

void PointAndClickPlayerWalkRightState::OnExit(PointAndClickPlayerFSMComponent *fsm)
{

}

// walk down state
void PointAndClickPlayerWalkDownState::OnEnter(PointAndClickPlayerFSMComponent *fsm)
{
	if (fsm->GetOwner()->HasComponents<SpriteAnimationComponent, SpriteComponent>())
		fsm->GetOwner()->GetComponent<SpriteAnimationComponent>()->PlayAnimation("walk_down", true);  // #TODO_decouple
}

void PointAndClickPlayerWalkDownState::OnExit(PointAndClickPlayerFSMComponent *fsm)
{

}

// walk left state
void PointAndClickPlayerWalkLeftState::OnEnter(PointAndClickPlayerFSMComponent *fsm)
{
	if (fsm->GetOwner()->HasComponents<SpriteAnimationComponent, SpriteComponent>())
		fsm->GetOwner()->GetComponent<SpriteAnimationComponent>()->PlayAnimation("walk_left", true);  // #TODO_decouple
}

void PointAndClickPlayerWalkLeftState::OnExit(PointAndClickPlayerFSMComponent *fsm)
{

}
