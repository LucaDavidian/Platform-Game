#include "PhysicsSystem.h"
#include "../Scene.h"
#include "../Entity.h"
#include "../components/TransformComponent.h"
#include "../components/PhysicsComponent.h"
#include "../../event system/EventBus.hpp"
#include "../../event system/Event.hpp"

void PhysicsSystem::Update(float dt)
{
	for (Entity *entity : Scene::GetInstance().GetEntities())
		if (entity->HasComponents<TransformComponent, PhysicsComponent>())
		{
			auto physicsComponent = entity->GetComponent<PhysicsComponent>();
			auto transformComponent = entity->GetComponent<TransformComponent>();

			// semi-implicit Euler integration
			glm::vec2 velocity = physicsComponent->GetVelocity();

			if (physicsComponent->HasMass())
			{
				glm::vec2 deltaVelocity(0.0f, -mGravity * dt / 1000.0f);
				physicsComponent->SetVelocity(velocity + deltaVelocity);
			}

			glm::vec2 deltaPosition = physicsComponent->GetVelocity() * dt / 1000.0f;
			transformComponent->SetPosition(transformComponent->GetPosition() + glm::vec3(deltaPosition, 0.0f));
		}
}

