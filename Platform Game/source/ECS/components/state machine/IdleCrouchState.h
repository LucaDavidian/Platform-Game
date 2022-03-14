#ifndef IDLECROUCHSTATE_H
#define IDLECROUCHSTATE_H

#include "HFSMComponent.h";

class IdleCrouchState : public HFSMComponent::HFSMState
{
public:
	IdleCrouchState(HFSMComponent &HFSM, const std::string &animationName) : HFSMComponent::HFSMState(HFSM), mAnimationName(animationName) {}

	void OnEnter() override;
	void OnUpdate(float deltaTime) override;
private:
	std::string mAnimationName;
};

#endif // IDLECROUCHSTATE_H