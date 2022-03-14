#include "IdleState.h"
#include "../../Entity.h"
#include "../../components/PlayerInputComponent.h"
#include "../../components/PhysicsComponent.h"
#include "../../components/SpriteAnimationComponent.h"

void IdleState::OnEnter()
{
	if (auto *physicsComponent = mHFSM.GetOwner()->GetComponent<PhysicsComponent>())
		physicsComponent->SetVelocity(glm::vec2(0.0f, 0.0f));

	if (auto *spriteAnimationComponent = mHFSM.GetOwner()->GetComponent<SpriteAnimationComponent>())
		spriteAnimationComponent->PlayAnimation(mAnimationName, true);
}

void IdleState::OnUpdate(float deltaTime)
{
	if (auto *inputComponent = mHFSM.GetOwner()->GetComponent<PlayerInputComponent>())
	{
		if (inputComponent->GetMove())
			mHFSM.ChangeState(this, "Move");

		if (inputComponent->IsCrouched())
			mHFSM.ChangeState(this, "IdleCrouch");
	}

	HFSMComponent::HFSMState::OnUpdate(deltaTime);
}
