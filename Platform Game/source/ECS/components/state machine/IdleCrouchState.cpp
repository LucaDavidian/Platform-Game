#include "IdleCrouchState.h"
#include "HFSMComponent.h"
#include "../SpriteAnimationComponent.h"
#include "../PlayerInputComponent.h"
#include "../PhysicsComponent.h"

void IdleCrouchState::OnEnter()
{
	if (auto *physicsComponent = mHFSM.GetOwner()->GetComponent<PhysicsComponent>())
		physicsComponent->SetVelocity(glm::vec2(0.0f, 0.0f));

	if (auto *spriteAnimationComponent = mHFSM.GetOwner()->GetComponent<SpriteAnimationComponent>())
		spriteAnimationComponent->PlayAnimation(mAnimationName, true);
}

void IdleCrouchState::OnUpdate(float deltaTime)
{
	if (auto *inputComponent = mHFSM.GetOwner()->GetComponent<PlayerInputComponent>())
	{
		if (!inputComponent->IsCrouched())
			mHFSM.ChangeState(this, "Idle");

		if (inputComponent->GetMove())
			mHFSM.ChangeState(this, "MoveCrouch");
	}

	HFSMComponent::HFSMState::OnUpdate(deltaTime);
}
