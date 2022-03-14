#ifndef FALLSTATE_H
#define FALLSTATE_H

#include "HFSMComponent.h";

class FallState : public HFSMComponent::HFSMState
{
public:
	FallState(HFSMComponent &HFSM, const std::string &animationName, bool loop = false) : HFSMComponent::HFSMState(HFSM), mAnimationName(animationName) {}

	void OnEnter() override;
	void OnUpdate(float deltaTime) override;

private:
	float mCoyoteTime = 200.0f;  // before coyote time expires character can still jump
	std::string mAnimationName;
};

#endif // FALLSTATE_H