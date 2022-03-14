#include "FallState.h"
#include "../../Entity.h"
#include "../../components/PlayerInputComponent.h"
#include "../../components/PhysicsComponent.h"
#include "../../components/SpriteAnimationComponent.h"
#include "../../../timer/Timer.h"

void FallState::OnEnter()
{
	if (auto *spriteAnimationComponent = mHFSM.GetOwner()->GetComponent<SpriteAnimationComponent>())
		spriteAnimationComponent->PlayAnimation(mAnimationName, true);
}

void FallState::OnUpdate(float deltaTime)
{
	auto *inputComponent = mHFSM.GetOwner()->GetComponent<PlayerInputComponent>();
	auto *physicsComponent = mHFSM.GetOwner()->GetComponent<PhysicsComponent>();

	//if (Timer::GlobalTimer().GetTime() - mStartTime < mCoyoteTime)
	//	if (inputComponent && physicsComponent)
	//		if (inputComponent->GetJump())
	//			mHFSM.ChangeState(this, "Jump");

	HFSMComponent::HFSMState::OnUpdate(deltaTime);
}
