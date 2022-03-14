#ifndef CHARACTERSTATE_H
#define CHARACTERSTATE_H

#include "HFSMComponent.h";

class CharacterState : public HFSMComponent::HFSMState
{
public:
	CharacterState(HFSMComponent &HFSM) : HFSMComponent::HFSMState(HFSM) {}
	
	void OnUpdate(float deltaTime) override;
};

#endif 
