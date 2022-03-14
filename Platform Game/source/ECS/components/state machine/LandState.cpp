#include "LandState.h"
#include "../../Entity.h"
#include "../../components/PlayerInputComponent.h"
#include "../../components/SpriteAnimationComponent.h"
#include "../../systems/AudioSystem.h"

void LandState::OnEnter()
{
	AudioSystem::GetInstance().PlayAudio("land");

	if (auto *spriteAnimationComponent = mHFSM.GetOwner()->GetComponent<SpriteAnimationComponent>())
		spriteAnimationComponent->PlayAnimation(mAnimationName);
}

void LandState::OnUpdate(float deltaTime)
{
	if (auto *inputComponent = mHFSM.GetOwner()->GetComponent<PlayerInputComponent>(); inputComponent && inputComponent->GetMove())
		mHFSM.ChangeState(this, "Move");

	if (auto *inputComponent = mHFSM.GetOwner()->GetComponent<PlayerInputComponent>(); inputComponent && inputComponent->IsCrouched())
		mHFSM.ChangeState(this, "IdleCrouch");

	if (auto *spriteAnimationComponent = mHFSM.GetOwner()->GetComponent<SpriteAnimationComponent>(); spriteAnimationComponent && spriteAnimationComponent->GetCurrentAnimation().empty())
		mHFSM.ChangeState(this, "Idle");

	HFSMComponent::HFSMState::OnUpdate(deltaTime);
}



