#ifndef STATEMACHINESYSTEM_H
#define STATEMACHINESYSTEM_H

#include "../Scene.h"
#include "../components/state machine/HFSMComponent.h"

class StateMachineSystem
{
public:
	static StateMachineSystem &GetInstance()
	{
		static StateMachineSystem instance;
		return instance;
	}

	void Update(float dt)
	{
		auto entities = Scene::GetInstance().GetEntities();

		for (int i = 0; i < entities.Size() - 1; i++)
			if (entities[i]->HasComponents<HFSMComponent>())
			{
				auto *hfsmComponent = entities[i]->GetComponent<HFSMComponent>();
				hfsmComponent->Update(dt);
			}
	}
private:
	StateMachineSystem() = default;
};

#endif // STATEMACHINESYSTEM_H