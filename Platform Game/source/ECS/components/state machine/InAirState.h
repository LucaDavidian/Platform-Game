#ifndef INAIRSTATE_H
#define INAIRSTATE_H

#include "HFSMComponent.h"

class InAirState : public HFSMComponent::HFSMState
{
public:
	InAirState(HFSMComponent &HFSM) : HFSMComponent::HFSMState(HFSM) {}

	void OnUpdate(float deltaTime) override;
	void OnExit() override;

private:
	float mMoveSpeed = 1.0f;
};

#endif // INAIRSTATE_H
