#include "InputComponent.h"
#include "../../Lua bindings/LuaVM.h"
#include "../Entity.h"
#include <SDL2/SDL.h>

namespace
{
	InputComponent InputComponentTableCtor(LuaObject componentTable)
	{
		return InputComponent();
	}

	void InputComponentAddToEntity(InputComponent const &inputComponent, Entity &entity)
	{
		entity.AddComponent<InputComponent>(inputComponent);
	}

	const bool registered = []() {
		Reflect::Reflect<InputComponent>("input")
			.AddConstructor(InputComponentTableCtor)
			.AddMemberFunction(InputComponentAddToEntity, "add_to_entity");

		return true;
	}();
}

void InputComponent::Update()
{
	for (auto &actionEventPair : mActionEvents)
		if (InputManager::GetInstance().CheckAction(actionEventPair.first))
			actionEventPair.second();
}