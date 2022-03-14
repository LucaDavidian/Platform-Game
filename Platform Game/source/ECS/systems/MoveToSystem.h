#ifndef MOVETO_SYSTEM_H
#define MOVETO_SYSTEM_H

#include "../Scene.h"
#include "../Entity.h"
#include "../components/PhysicsComponent.h"
#include "../components/MoveToComponent.h"
#include "../components/TransformComponent.h"

class MoveToSystem
{
public:
	static MoveToSystem &GetInstance()
	{
		static MoveToSystem instance;
		return instance;
	}

	void Update(float dt)
	{
		for (auto entity : Scene::GetInstance().GetEntities())
			if (entity->HasComponents<PhysicsComponent, MoveToComponent, TransformComponent>())
			{
				auto transformComponent = entity->GetComponent<TransformComponent>();
				auto physicsComponent = entity->GetComponent<PhysicsComponent>();
				auto moveToComponent = entity->GetComponent<MoveToComponent>();

				auto wayPoint = moveToComponent->GetWayPoint();
				float speed = moveToComponent->GetSpeed();

				auto currentPosition = transformComponent->GetPosition();
				if (glm::dot(wayPoint.end - wayPoint.start, wayPoint.end - glm::vec2(currentPosition.x, currentPosition.y)) <= 0)
				{
					physicsComponent->SetVelocity(glm::vec2(0.0f));
					moveToComponent->NextWaypoint();
					wayPoint = moveToComponent->GetWayPoint();
				}
				else
				{
					auto direction = glm::normalize(wayPoint.end - wayPoint.start);
					physicsComponent->SetVelocity(direction * moveToComponent->GetSpeed() / 20.0f);
				}
			}
	}
private:
	MoveToSystem() = default;
};

#endif // MOVETO_SYSTEM_H
