#include "PhysicsComponent.h"
#include "../../Lua bindings/LuaVM.h"
#include "../Entity.h"

namespace
{
	PhysicsComponent PhysicsComponentTableCtor(LuaObject componentTable)
	{
		if (auto mass = componentTable.Get("mass"); mass)
			return PhysicsComponent(mass.Cast<float>());
		else
			return PhysicsComponent();
	}

	void PhysicsComponentAddToEntity(PhysicsComponent const &physicsComponent, Entity &entity)
	{
		entity.AddComponent<PhysicsComponent>(physicsComponent);
	}
}

const bool PhysicsComponent::mRegistered = []() {
	Reflect::Reflect<PhysicsComponent>("physics")
		.AddConstructor(PhysicsComponentTableCtor)
		.AddMemberFunction(PhysicsComponentAddToEntity, "add_to_entity");

	return true;
}();

