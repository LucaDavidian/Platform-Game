#include "AttackState.h"
#include "../../Entity.h"
#include "../../components/SpriteAnimationComponent.h"
#include "../../components/PhysicsComponent.h"
#include "../../components/PlayerInputComponent.h"
#include "../../components/SpriteComponent.h"
#include "../../systems/AudioSystem.h"

void AttackState::OnEnter()
{
	AudioSystem::GetInstance().PlayAudio("attack1");

	if (auto *physicsComponent = mHFSM.GetOwner()->GetComponent<PhysicsComponent>())
		physicsComponent->SetVelocity(glm::vec2(0.0f, 0.0f));

	if (auto *spriteAnimationComponent = mHFSM.GetOwner()->GetComponent<SpriteAnimationComponent>())
		if (auto *inputComponent = mHFSM.GetOwner()->GetComponent<PlayerInputComponent>())
		{
			int attackType = inputComponent->GetAttack(true);
			if (attackType >= 0)
			{
				spriteAnimationComponent->PlayAnimation(mAnimationNames[attackType]);

				if (auto *spriteComponent = mHFSM.GetOwner()->GetComponent<SpriteComponent>())
				{
					// check if damageable entity is in range

				}
			}
		}
}

void AttackState::OnUpdate(float deltaTime)
{
	if (auto *spriteAnimationComponent = mHFSM.GetOwner()->GetComponent<SpriteAnimationComponent>(); spriteAnimationComponent->GetCurrentAnimation().empty())
		mHFSM.ChangeState(this, "Idle");

	HFSMComponent::HFSMState::OnUpdate(deltaTime);
}


