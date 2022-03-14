#ifndef MOVESTATE_H
#define MOVESTATE_H

#include "HFSMComponent.h";

class MoveState : public HFSMComponent::HFSMState
{
public:
	MoveState(HFSMComponent &HFSM, const std::string &animationName) : HFSMComponent::HFSMState(HFSM), mAnimationName(animationName) {}

	void OnEnter() override;
	void OnUpdate(float deltaTime) override;

private:
	float mMoveSpeed = 1.0f;
	std::string mAnimationName;
};

#endif // MOVESTATE_H