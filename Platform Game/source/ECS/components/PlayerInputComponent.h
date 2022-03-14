#ifndef PLAYERINPUT_COMPONENT_H
#define PLAYERINPUT_COMPONENT_H

#include "InputComponent.h"
#include <glm/glm.hpp>

class PlayerInputComponent : public InputComponent
{
public:
	PlayerInputComponent() {}

	void Init() override;
	void Update() override
	{
		StopMove();
		mCrouch = false;  // TODO input component must sense when action button is not held down

		InputComponent::Update();
	}

	void SetMoveRight() { mMove = 1; }
	void SetMoveLeft() { mMove = -1; }
	void StopMove() { mMove = 0; }
	void SetJump() { mJump = true; }
	void SetAttack1() { mAttack = 0; }
	void SetAttack2() { mAttack = 1; }
	void SetAttack3() { mAttack = 2; }
	void Crouch() { mCrouch = true; }
	bool IsCrouched() const { return mCrouch; }

	int GetMove() const { return mMove; }
	bool GetJump() { bool jump = mJump; mJump = false; return jump; }  // consume input
	int GetAttack(bool consume = false) 
	{ 
		int attack = mAttack; 
		
		if (consume)
			mAttack = -1; 

		return attack; 
	}

private:
	int mMove = 0;
	bool mJump = false;
	int mAttack = -1;
	bool mCrouch = false;
};

#endif  // PLAYERINPUT_COMPONENT_H