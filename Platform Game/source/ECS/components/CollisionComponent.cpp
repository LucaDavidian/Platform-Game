#include "CollisionComponent.h"
#include "../../Lua bindings/LuaVM.h"
#include "../Entity.h"

namespace
{
	CollisionComponent CollisionComponentTableCtor(LuaObject componentTable)
	{
		LuaObject offset = componentTable.Get("offset");
		LuaObject dimensions = componentTable.Get("dimensions");

		glm::vec2 offsetVec = glm::vec2(0.0f);
		glm::vec2 dimensionsVec = glm::vec2(1.0f, 1.0f);
		
		if (offset)
		{
			offsetVec.x = offset.Get(1);
			offsetVec.y = offset.Get(2);
		}
		
		if (dimensions)
		{
			dimensionsVec.x = dimensions.Get(1);
			dimensionsVec.y = dimensions.Get(2);
		}

		return CollisionComponent(offsetVec, dimensionsVec);
	}

	void CollisionComponentAddToEntity(CollisionComponent const &collisionComponent, Entity &entity)
	{
		entity.AddComponent<CollisionComponent>(collisionComponent);
	}

	static const int registered = []() {
		Reflect::Reflect<CollisionComponent>("collision")
			.AddConstructor(CollisionComponentTableCtor)
			.AddMemberFunction(CollisionComponentAddToEntity, "add_to_entity");

		return 1;
	}();
}



