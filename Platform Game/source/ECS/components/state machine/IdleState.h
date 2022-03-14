#ifndef IDLESTATE_H
#define IDLESTATE_H

#include "HFSMComponent.h";

class IdleState : public HFSMComponent::HFSMState
{
public:
	IdleState(HFSMComponent &HFSM, const std::string &animationName) : HFSMComponent::HFSMState(HFSM), mAnimationName(animationName) {}

	void OnEnter() override;
	void OnUpdate(float deltaTime) override;
private:
	std::string mAnimationName;
};

#endif // IDLESTATE_H
