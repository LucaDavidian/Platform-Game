#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include "../Scene.h"
#include "../components/InputComponent.h"
#include "../components/PlayerInputComponent.h"

class InputSystem
{
public:
	static InputSystem &GetInstance()
	{
		static InputSystem instance;
		return instance;
	}

	void Update()
	{
		auto entities = Scene::GetInstance().GetEntities();

		for (auto &entity : entities)
		{
			if (entity->HasComponent<InputComponent>())
				entity->GetComponent<InputComponent>()->Update();

			if (entity->HasComponent<PlayerInputComponent>())   // #TODO_not_quite_right
				entity->GetComponent<PlayerInputComponent>()->Update();
		}
	}

private:
	InputSystem() = default;
};

#endif // INPUTSYSTEM_H
