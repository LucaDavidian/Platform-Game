#include "MoveCrouchState.h"
#include "HFSMComponent.h"
#include "../SpriteAnimationComponent.h"
#include "../PlayerInputComponent.h"
#include "../PhysicsComponent.h"
#include "../SpriteComponent.h"

void MoveCrouchState::OnEnter()
{
	if (auto *spriteAnimationComponent = mHFSM.GetOwner()->GetComponent<SpriteAnimationComponent>())
		spriteAnimationComponent->PlayAnimation(mAnimationName, true);
}

void MoveCrouchState::OnUpdate(float deltaTime)
{
	auto *physicsComponent = mHFSM.GetOwner()->GetComponent<PhysicsComponent>();
	auto *spriteComponent = mHFSM.GetOwner()->GetComponent <SpriteComponent>();
	auto *inputComponent = mHFSM.GetOwner()->GetComponent<PlayerInputComponent>();

	if (inputComponent)
	{
		if (!inputComponent->IsCrouched())
			mHFSM.ChangeState(this, "Move");

		if (physicsComponent)
			if (inputComponent->GetMove())
			{
				physicsComponent->SetVelocity(glm::vec2(inputComponent->GetMove() * mMoveSpeed, physicsComponent->GetVelocity().y));

				if (spriteComponent)
					switch (inputComponent->GetMove())
					{
					case 1:
						spriteComponent->Flip(false);
						break;
					case -1:
						spriteComponent->Flip(true);
						break;
					}
			}
			else
				mHFSM.ChangeState(this, "IdleCrouch");
	}

	HFSMComponent::HFSMState::OnUpdate(deltaTime);
}
