#include "InAirState.h"
#include "../../Entity.h"
#include "../../components/CollisionComponent.h"
#include "../../components/PlayerInputComponent.h"
#include "../../components/PhysicsComponent.h"
#include "../../components/SpriteComponent.h"

void InAirState::OnUpdate(float deltaTime)
{
	// move while in air
	auto *inputComponent = mHFSM.GetOwner()->GetComponent<PlayerInputComponent>();
	auto *physicsComponent = mHFSM.GetOwner()->GetComponent<PhysicsComponent>();
	auto *spriteComponent = mHFSM.GetOwner()->GetComponent<SpriteComponent>();

	if (inputComponent && physicsComponent)
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
			physicsComponent->SetVelocity(glm::vec2(0.0f, physicsComponent->GetVelocity().y));

	// check if on ground in collision component
	if (auto *collisionComponent = mHFSM.GetOwner()->GetComponent<CollisionComponent>(); collisionComponent->IsOnGround())
		mHFSM.ChangeState(this, "Land");

	HFSMComponent::HFSMState::OnUpdate(deltaTime);
}

void InAirState::OnExit()
{
	if (auto *inputComponent = mHFSM.GetOwner()->GetComponent<PlayerInputComponent>())
	{
		(void)inputComponent->GetJump();  // consume in-air jumps
		(void)inputComponent->GetAttack(true);  // consume in-air attacks
	}
}
