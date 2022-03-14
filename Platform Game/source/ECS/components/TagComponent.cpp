#include "TagComponent.h"
#include "../../Lua bindings/LuaVM.h"
#include "../Entity.h"

namespace
{
	TagComponent TagComponentTableCtor(LuaObject componentTable)
	{
		LuaObject tag = componentTable.Get(1);

		return TagComponent(tag.Cast<std::string>());
	}

	void TagComponentAddToEntity(TagComponent tagComponent, Entity &entity)
	{
		entity.AddComponent<TagComponent>(tagComponent);
	}
}

const bool TagComponent::mRegistered = []() {
	Reflect::Reflect<TagComponent>("tag")
		.AddConstructor(TagComponentTableCtor)
		.AddMemberFunction(TagComponentAddToEntity, "add_to_entity");

	return true;
}();

