#include "OnGroundState.h"
#include "../../Entity.h"
#include "../../components/PlayerInputComponent.h"
#include "../../components/CollisionComponent.h"

void OnGroundState::OnUpdate(float deltaTime)
{
	if (auto *inputComponent = mHFSM.GetOwner()->GetComponent<PlayerInputComponent>(); inputComponent && inputComponent->GetJump())
		mHFSM.ChangeState(this, "Jump");
	
	if (auto *inputComponent = mHFSM.GetOwner()->GetComponent<PlayerInputComponent>(); inputComponent && inputComponent->GetAttack(false) >= 0)
		mHFSM.ChangeState(this, "Attack");

	if (auto *collisionComponent = mHFSM.GetOwner()->GetComponent<CollisionComponent>(); collisionComponent && !collisionComponent->IsOnGround())
		mHFSM.ChangeState(this, "Fall");
	
	HFSMComponent::HFSMState::OnUpdate(deltaTime);
}
