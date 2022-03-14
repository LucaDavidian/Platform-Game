#ifndef JUMPSTATE_H
#define JUMPSTATE_H

#include "HFSMComponent.h";

class JumpState : public HFSMComponent::HFSMState
{
public:
	JumpState(HFSMComponent &HFSM, const std::string &animationName) : HFSMComponent::HFSMState(HFSM), mAnimationName(animationName) {} 

	void OnEnter() override;
	void OnUpdate(float deltaTime) override;

private:
	float mJumpSpeed = 1.0f;
	std::string mAnimationName;
};

#endif // JUMPSTATE_H