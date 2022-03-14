#ifndef MOVECROUCHSTATE_H
#define MOVECROUCHSTATE_H

#include "HFSMComponent.h";

class MoveCrouchState : public HFSMComponent::HFSMState
{
public:
	MoveCrouchState(HFSMComponent &HFSM, const std::string &animationName) : HFSMComponent::HFSMState(HFSM), mAnimationName(animationName) {}

	void OnEnter() override;
	void OnUpdate(float deltaTime) override;
private:
	std::string mAnimationName;
	float mMoveSpeed = 0.3f;
};

#endif // MOVECROUCHSTATE_H