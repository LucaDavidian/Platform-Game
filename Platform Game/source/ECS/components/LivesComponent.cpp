#include "LivesComponent.h"
#include "../../Lua bindings/LuaVM.h"
#include "../Entity.h"

namespace
{
	LivesComponent LivesComponentTableCtor(LuaObject componentTable)
	{
		auto lives = componentTable.Get(1);

		float numLives = lives ? lives.Cast<unsigned int>() : 4U;
		return LivesComponent(numLives);
	}

	void LivesComponentAddToEntity(LivesComponent const &livesComponent, Entity &entity)
	{
		entity.AddComponent<LivesComponent>(livesComponent);
	}
}

const bool LivesComponent::mRegistered = []() {
	Reflect::Reflect<LivesComponent>("lives")
		.AddConstructor(LivesComponentTableCtor)
		.AddMemberFunction(LivesComponentAddToEntity, "add_to_entity");

	return true;
}();

