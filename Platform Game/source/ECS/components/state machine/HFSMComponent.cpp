#include "HFSMComponent.h"
#include "../../../Lua bindings/LuaVM.h"
#include "../../Entity.h"
#include "IdleState.h"
#include "MoveState.h"
#include "JumpState.h"
#include "LandState.h"
#include "FallState.h"
#include "AttackState.h"
#include "OnGroundState.h"
#include "InAirState.h"
#include "CharacterState.h"
#include "IdleCrouchState.h"
#include "MoveCrouchState.h"

namespace
{
	HFSMComponent HFSMComponentTableCtor(LuaObject componentTable)
	{
		return HFSMComponent();
	}

	void HFSMComponentAddToEntity(HFSMComponent const &hfsmComponent, Entity &entity)
	{
		entity.AddComponent<HFSMComponent>(hfsmComponent);

		auto *hfsm = entity.GetComponent<HFSMComponent>();

		CharacterState *characterState = new CharacterState(*hfsm);
		
		OnGroundState *onGroundState = new OnGroundState(*hfsm);
		IdleState *idleState = new IdleState(*hfsm, "idle");
		IdleCrouchState *idleCrouchState = new IdleCrouchState(*hfsm, "idle_crouch");
		MoveState *moveState = new MoveState(*hfsm, "move");
		MoveCrouchState *moveCrouchState = new MoveCrouchState(*hfsm, "move_crouch");
		LandState *landState = new LandState(*hfsm, "land");

		InAirState *inAirState = new InAirState(*hfsm);
		JumpState *jumpState = new JumpState(*hfsm, "jump");
		FallState *fallState = new FallState(*hfsm, "fall", true);

		AttackState *attackState = new AttackState(*hfsm, { "attack1", "attack2", "attack3" });

		onGroundState->SetParent(characterState);

		idleState->SetParent(onGroundState);
		idleCrouchState->SetParent(onGroundState);
		moveState->SetParent(onGroundState);
		moveCrouchState->SetParent(onGroundState);
		landState->SetParent(onGroundState);
		
		inAirState->SetParent(characterState);

		jumpState->SetParent(inAirState);
		fallState->SetParent(inAirState);

		attackState->SetParent(characterState);

		characterState->SetInitialState(onGroundState);
		onGroundState->SetInitialState(idleState);
		inAirState->SetInitialState(fallState);

		hfsm->AddState("Character", characterState);

		hfsm->AddState("OnGround", onGroundState);   // #TODO_from_script
		hfsm->AddState("Idle", idleState); 
		hfsm->AddState("IdleCrouch", idleCrouchState);
		hfsm->AddState("Move", moveState);
		hfsm->AddState("MoveCrouch", moveCrouchState);
		hfsm->AddState("Land", landState);

		hfsm->AddState("InAir", inAirState);
		hfsm->AddState("Jump", jumpState);
		hfsm->AddState("Fall", fallState);
		
		hfsm->AddState("Attack", attackState);

		hfsm->Initialize();
	}

	bool mRegistered = []() {
		Reflect::Reflect<HFSMComponent>("FSM")
			.AddConstructor(HFSMComponentTableCtor)
			.AddMemberFunction(HFSMComponentAddToEntity, "add_to_entity");

		return true;
	}();
}


