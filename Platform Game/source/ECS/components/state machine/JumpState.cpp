#include "JumpState.h"
#include "../../Entity.h"
#include "../../components/PhysicsComponent.h"
#include "../../components/SpriteAnimationComponent.h"
#include "../../systems/AudioSystem.h"

void JumpState::OnEnter()
{
	AudioSystem::GetInstance().PlayAudio("jump"); 

	if (auto *spriteAnimationComponent = mHFSM.GetOwner()->GetComponent<SpriteAnimationComponent>())
		spriteAnimationComponent->PlayAnimation(mAnimationName);

	if (auto *physicsComponent = mHFSM.GetOwner()->GetComponent<PhysicsComponent>())
		physicsComponent->SetVelocity(physicsComponent->GetVelocity() + glm::vec2(0.0f, 1.0f) * mJumpSpeed);
}

void JumpState::OnUpdate(float deltaTime)
{
	// check when jump animation finished
	//if (auto *spriteAnimationComponent = mHFSM.GetOwner()->GetComponent<SpriteAnimationComponent>(); spriteAnimationComponent->GetCurrentAnimation().empty())
	//	mHFSM.ChangeState(this, "Fall");

	// check when falling
	if (auto *physicsComponent = mHFSM.GetOwner()->GetComponent<PhysicsComponent>())
		if (physicsComponent->GetVelocity().y <= 0)
			mHFSM.ChangeState(this, "Fall");

	HFSMComponent::HFSMState::OnUpdate(deltaTime);
}


