#ifndef LANDSTATE_H
#define LANDSTATE_H

#include "HFSMComponent.h";

class LandState : public HFSMComponent::HFSMState
{
public:
	LandState(HFSMComponent &HFSM, const std::string &animationName) : HFSMComponent::HFSMState(HFSM), mAnimationName(animationName) {}
	
	void OnEnter() override;
	void OnUpdate(float deltaTime) override;
private:
	std::string mAnimationName;
};

#endif // LANDSTATE_H
