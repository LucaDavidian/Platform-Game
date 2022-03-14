#include "MoveState.h"
#include "../../components/PhysicsComponent.h"
#include "../../components/SpriteComponent.h"
#include "../../components/PlayerInputComponent.h"
#include "../../components/SpriteAnimationComponent.h"

void MoveState::OnEnter()
{
	if (auto *spriteAnimationComponent = mHFSM.GetOwner()->GetComponent<SpriteAnimationComponent>())
		spriteAnimationComponent->PlayAnimation(mAnimationName, true);
}

void MoveState::OnUpdate(float deltaTime)
{
	auto *physicsComponent = mHFSM.GetOwner()->GetComponent<PhysicsComponent>();
	auto *spriteComponent = mHFSM.GetOwner()->GetComponent <SpriteComponent>();
	auto *inputComponent = mHFSM.GetOwner()->GetComponent<PlayerInputComponent>();

	if (inputComponent)
	{
		if (inputComponent->IsCrouched())
			mHFSM.ChangeState(this, "MoveCrouch");

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
				mHFSM.ChangeState(this, "Idle");
	}		

	HFSMComponent::HFSMState::OnUpdate(deltaTime);
}
