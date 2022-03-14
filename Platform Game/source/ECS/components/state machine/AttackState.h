#ifndef ATTACKCSTATE_H
#define ATTACKSTATE_H

#include "HFSMComponent.h";
#include "../../../data structures/vector.hpp"

class AttackState : public HFSMComponent::HFSMState
{
public:
	AttackState(HFSMComponent &HFSM, const Vector<std::string> &animationNames) : HFSMComponent::HFSMState(HFSM), mAnimationNames(animationNames) {}

	void OnEnter() override;
	void OnUpdate(float deltaTime) override;
private:
	Vector<std::string> mAnimationNames;
};

#endif // ATTACKSTATE_H