#include "MoveToComponent.h"
#include "../../Lua bindings/LuaVM.h"
#include "../Entity.h"

namespace
{
	MoveToComponent MoveToComponentTableCtor(LuaObject componentTable)
	{
		Vector<glm::vec2> points;
		Vector<MoveToComponent::WayPoint> wayPoints;

		while (componentTable.HasNext())
		{
			auto wayPointTable = componentTable.GetNext().second;
			glm::vec2 point(wayPointTable.Get(1), wayPointTable.Get(2));
			points.InsertLast(point);
		}

		for (int i = 0; i < points.Size(); i++)
		{
			MoveToComponent::WayPoint wayPoint{ glm::vec2(points[i].x, points[i].y), glm::vec2(points[(i + 1) % points.Size()].x, points[(i + 1) % points.Size()].y) };
			wayPoints.InsertLast(wayPoint);
		}

		return MoveToComponent(wayPoints);
	}

	void MoveToComponentAddToEntity(MoveToComponent const &moveToComponent, Entity &entity)
	{
		entity.AddComponent<MoveToComponent>(moveToComponent);
	}
}

const bool MoveToComponent::mRegistered = []() {
	Reflect::Reflect<MoveToComponent>("move_to")
		.AddConstructor(MoveToComponentTableCtor)
		.AddMemberFunction(MoveToComponentAddToEntity, "add_to_entity");

	return true;
}();

