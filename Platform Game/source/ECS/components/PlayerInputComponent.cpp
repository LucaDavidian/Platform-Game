#include "PlayerInputComponent.h"
#include "../../Lua bindings/LuaVM.h"
#include "../Entity.h"
#include <SDL2/SDL.h>

namespace
{
	PlayerInputComponent PlayerInputComponentTableCtor(LuaObject componentTable)
	{
		return PlayerInputComponent();
	}

	void PlayerInputComponentAddToEntity(PlayerInputComponent const &inputComponent, Entity &entity)
	{
		entity.AddComponent<PlayerInputComponent>(inputComponent);

		// #TODO: key bindings should be defined from config file or Lua script
		InputManager::GetInstance().AddKeyBinding("move right", { {SDL_SCANCODE_D, InputManager::KeyState::PRESSED}/*, {SDL_SCANCODE_LCTRL, InputManager::KeyState::PRESSED}*/ });
		InputManager::GetInstance().AddKeyBinding("move left", { {SDL_SCANCODE_A, InputManager::KeyState::PRESSED}/*, {SDL_SCANCODE_LCTRL, InputManager::KeyState::PRESSED}*/ });
		//InputManager::GetInstance().AddKeyBinding("stop move right", { {SDL_SCANCODE_D, InputManager::KeyState::JUST_RELEASED} });
		//InputManager::GetInstance().AddKeyBinding("stop move left", { {SDL_SCANCODE_A, InputManager::KeyState::JUST_RELEASED} });
		InputManager::GetInstance().AddKeyBinding("jump", { {SDL_SCANCODE_S, InputManager::KeyState::JUST_PRESSED} });
		InputManager::GetInstance().AddKeyBinding("attack1", { {SDL_SCANCODE_B, InputManager::KeyState::JUST_PRESSED} });
		InputManager::GetInstance().AddKeyBinding("attack2", { {SDL_SCANCODE_V, InputManager::KeyState::JUST_PRESSED} });
		InputManager::GetInstance().AddKeyBinding("attack3", { {SDL_SCANCODE_N, InputManager::KeyState::JUST_PRESSED} });
		InputManager::GetInstance().AddKeyBinding("crouch", { {SDL_SCANCODE_C, InputManager::KeyState::PRESSED} });
	}

	const bool registered = []() {
		Reflect::Reflect<PlayerInputComponent>("player_input")
			.AddConstructor(PlayerInputComponentTableCtor)
			.AddMemberFunction(PlayerInputComponentAddToEntity, "add_to_entity");

		return true;
	}();
}

void PlayerInputComponent::Init() 
{
	BindAction("move right", *this, &PlayerInputComponent::SetMoveRight);      // #TODO_better_implementation pass key state to bind (see UE4) ?
	BindAction("move left", *this, &PlayerInputComponent::SetMoveLeft);
	//BindAction("stop move right", *this, &PlayerInputComponent::StopMove);
	//BindAction("stop move left", *this, &PlayerInputComponent::StopMove);
	BindAction("jump", *this, &PlayerInputComponent::SetJump);
	BindAction("attack1", *this, &PlayerInputComponent::SetAttack1);   // #TODO pass values in action handlers
	BindAction("attack2", *this, &PlayerInputComponent::SetAttack2); 
	BindAction("attack3", *this, &PlayerInputComponent::SetAttack3);
	BindAction("crouch", *this, &PlayerInputComponent::Crouch);
}
