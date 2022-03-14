#ifndef ONGROUNDSTATE_H
#define ONGROUNDSTATE_H

#include "HFSMComponent.h"

class OnGroundState : public HFSMComponent::HFSMState
{
public:
	OnGroundState(HFSMComponent &HFSM) : HFSMComponent::HFSMState(HFSM) {} 

	void OnUpdate(float deltaTime) override;
};

#endif // ONGROUNDSTATE_H