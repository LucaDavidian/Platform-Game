#include "CollisionSystem.h"
#include "../Scene.h"
#include "../components/TransformComponent.h"
#include "../components/CollisionComponent.h"
#include "../components/PhysicsComponent.h"
#include "../components/TagComponent.h"
#include "../../event system/EventBus.hpp"
#include "../../event system/Event.hpp"

void CollisionSystem::Update(float dt)
{
	mPlayerOnPlatform = false;

	auto entities = Scene::GetInstance().GetEntities();

	for (int i = 0; i < entities.Size() - 1; i++)
		for (int j = i + 1; j < entities.Size(); j++)
		{
			if (entities[i]->HasComponents<TransformComponent, CollisionComponent>() && entities[j]->HasComponents<TransformComponent, CollisionComponent>())
			{
				auto transformComponent1 = entities[i]->GetComponent<TransformComponent>();
				auto collisionComponent1 = entities[i]->GetComponent<CollisionComponent>();

				auto transformComponent2 = entities[j]->GetComponent<TransformComponent>();
				auto collisionComponent2 = entities[j]->GetComponent<CollisionComponent>();

				glm::vec2 colliderPosition1 = glm::vec2(transformComponent1->GetPosition()) + collisionComponent1->GetOffset();
				glm::vec2 colliderDimensions1 = glm::vec2(collisionComponent1->GetDimensions().x * transformComponent1->GetScale().x, collisionComponent1->GetDimensions().y * transformComponent1->GetScale().y);

				glm::vec2 colliderPosition2 = glm::vec2(transformComponent2->GetPosition()) + collisionComponent2->GetOffset();
				glm::vec2 colliderDimensions2 = glm::vec2(collisionComponent2->GetDimensions().x * transformComponent2->GetScale().x, collisionComponent2->GetDimensions().y * transformComponent2->GetScale().y);

				glm::vec2 outContactPoint = glm::vec2(0.0f), outContactNormal = glm::vec2(0.0f);
				float outPenetration = 0.0f;

				// if SAT not passed (return true) a collision occurred
				// a collision event is triggered, which is handled differently by different objects
				// (player/enemy --> damage system, player/platform --> collision resolution)
				if (SAT({ colliderPosition1, colliderDimensions1 }, { colliderPosition2, colliderDimensions2 }, outContactPoint, outContactNormal, outPenetration))
				{
					ResolveCollision(entities[i], entities[j], outContactPoint, outContactNormal, outPenetration);  // #TODO_global_collision_resolution: store all collisions and perform collision resolution at the end
					gEventBus.TriggerEvent(CollisionEvent{ entities[i], entities[j], outContactPoint, outContactNormal, outPenetration });
				}
			}
		}

	// set player on ground - TODO: wrong way
	Entity *player = nullptr;

	for (Entity *entity : entities)
		if (entity->HasComponent<TagComponent>() && entity->GetComponent<TagComponent>()->GetTag() == "player")
			player = entity;

	if (!mPlayerOnPlatform)
	{
		gEventBus.TriggerEvent<PlayerNotOnPlatformEvent>();

		if (player)
			player->GetComponent<CollisionComponent>()->SetOnGround(false);
	}
	else if (player)
		player->GetComponent<CollisionComponent>()->SetOnGround(true);
}

bool CollisionSystem::SAT(Collider c1, Collider c2, glm::vec2 &contactPoint, glm::vec2 &contactNormal, float &penetration)
{
	// separating axis test for AABB (symmetric concave polygons)

	// x-axis
	{
		float distance = glm::abs(c1.position.x - c2.position.x);
		float a = c1.dimensions.x / 2.0f;
		float b = c2.dimensions.x / 2.0f;

		if (a + b <= distance)
			return false;
		else
		{
			penetration = a + b - distance;
			if (c1.position.x < c2.position.x)
				contactNormal = glm::vec2(-1.0f, 0.0f);   // contact normal is always directed from c2 to c1
			else
				contactNormal = glm::vec2(1.0f, 0.0f);
		}
	}

	// y-axis
	{
		float distance = glm::abs(c1.position.y - c2.position.y);
		float a = c1.dimensions.y / 2.0f;
		float b = c2.dimensions.y / 2.0f;

		if (a + b <= distance)
			return false;
		else if (a + b - distance < penetration)
		{
			penetration = a + b - distance;
			if (c1.position.y < c2.position.y)
				contactNormal = glm::vec2(0.0f, -1.0f);   // contact normal is always directed from c2 to c1
			else
				contactNormal = glm::vec2(0.0f, 1.0f);
		}
	}

	return true;
}

void CollisionSystem::ResolveCollision(Entity *entity1, Entity *entity2, glm::vec2 contactPoint, glm::vec2 contactNormal, float penetration)
{
	entity1->GetComponent<CollisionComponent>()->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
	entity2->GetComponent<CollisionComponent>()->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));

	// player/platform collision
	if (entity1->HasComponent<TagComponent>() && entity1->GetComponent<TagComponent>()->GetTag() == "player")
		if (entity2->HasComponent<TagComponent>() && entity2->GetComponent<TagComponent>()->GetTag() == "platform")
		{
			auto playerTransformComponent = entity1->GetComponent<TransformComponent>();
			auto playerPhysicsComponent = entity1->GetComponent<PhysicsComponent>();
			auto platformPhysicsComponent = entity2->GetComponent<PhysicsComponent>();

			// resolve interpenetration
			playerTransformComponent->SetPosition(playerTransformComponent->GetPosition() + glm::vec3(contactNormal * penetration, 0.0f));

			// resolve velocity
			if (contactNormal.x)
				playerPhysicsComponent->SetVelocity(glm::vec2(0.0f, playerPhysicsComponent->GetVelocity().y));
			else 
				if (contactNormal.y > 0)  // player above: contact normal is always directed as entity2 --> entity1
				{
					if (platformPhysicsComponent)  // moving platform
						if(platformPhysicsComponent->GetVelocity().y <= 0)
							playerPhysicsComponent->SetVelocity(glm::vec2(platformPhysicsComponent->GetVelocity().x, platformPhysicsComponent->GetVelocity().y));
						else
							playerPhysicsComponent->SetVelocity(glm::vec2(platformPhysicsComponent->GetVelocity().x, 0.0));
					else  // prop
						playerPhysicsComponent->SetVelocity(glm::vec2(playerPhysicsComponent->GetVelocity().x, 0.0f));

					entity1->GetEventBus().TriggerEvent<PlayerOnPlatformEvent>();
					mPlayerOnPlatform = true;
				}
				else  // event.contactNormal.y < 0 (player below)
					if (platformPhysicsComponent)  // moving platform
						if (platformPhysicsComponent->GetVelocity().y <= 0)
							playerPhysicsComponent->SetVelocity(glm::vec2(playerPhysicsComponent->GetVelocity().x, platformPhysicsComponent->GetVelocity().y));
						else
							playerPhysicsComponent->SetVelocity(glm::vec2(playerPhysicsComponent->GetVelocity().x, 0.0f));
					else  // prop
						playerPhysicsComponent->SetVelocity(glm::vec2(playerPhysicsComponent->GetVelocity().x, 0.0f));  
		}

	if (entity2->HasComponent<TagComponent>() && entity2->GetComponent<TagComponent>()->GetTag() == "player")
		if (entity1->HasComponent<TagComponent>() && entity1->GetComponent<TagComponent>()->GetTag() == "platform")
		{
			auto playerTransformComponent = entity2->GetComponent<TransformComponent>();
			auto playerPhysicsComponent = entity2->GetComponent<PhysicsComponent>();
			auto platformPhysicsComponent = entity1->GetComponent<PhysicsComponent>();

			// resolve interpenetration
			playerTransformComponent->SetPosition(playerTransformComponent->GetPosition() - glm::vec3(contactNormal * penetration, 0.0f));

			// resolve velocity
			if (contactNormal.x)
				playerPhysicsComponent->SetVelocity(glm::vec2(0.0f, playerPhysicsComponent->GetVelocity().y));
			else
				if (contactNormal.y < 0)  // player above: contact normal is always directed as entity2 --> entity1
				{
					if (platformPhysicsComponent)  // moving platform
						if (platformPhysicsComponent->GetVelocity().y <= 0)
							playerPhysicsComponent->SetVelocity(glm::vec2(platformPhysicsComponent->GetVelocity().x, platformPhysicsComponent->GetVelocity().y));
						else
							playerPhysicsComponent->SetVelocity(glm::vec2(platformPhysicsComponent->GetVelocity().x, 0.0f));
					else  // prop
						playerPhysicsComponent->SetVelocity(glm::vec2(playerPhysicsComponent->GetVelocity().x, 0.0f));

					entity2->GetEventBus().TriggerEvent<PlayerOnPlatformEvent>();
					mPlayerOnPlatform = true;
				}
				else  // event.contactNormal.y > 0 (player below)
					if (platformPhysicsComponent)  // moving platform
						if (platformPhysicsComponent->GetVelocity().y <= 0)
							playerPhysicsComponent->SetVelocity(glm::vec2(playerPhysicsComponent->GetVelocity().x, platformPhysicsComponent->GetVelocity().y));
						else
							playerPhysicsComponent->SetVelocity(glm::vec2(playerPhysicsComponent->GetVelocity().x, 0.0f));
					else  // prop
						playerPhysicsComponent->SetVelocity(glm::vec2(playerPhysicsComponent->GetVelocity().x, 0.0f));  
		}

	// player/enemy collision
	if (entity1->HasComponent<TagComponent>() && entity1->GetComponent<TagComponent>()->GetTag() == "player")
		if (entity2->HasComponent<TagComponent>() && entity2->GetComponent<TagComponent>()->GetTag() == "enemy")
		{
			auto playerTransformComponent = entity1->GetComponent<TransformComponent>();
			auto playerPhysicsComponent = entity1->GetComponent<PhysicsComponent>();
			auto enemyPhysicsComponent = entity2->GetComponent<PhysicsComponent>();

			// resolve interpenetration
			playerTransformComponent->SetPosition(playerTransformComponent->GetPosition() + glm::vec3(contactNormal * penetration * 1.2f, 0.0f));
		}

	if (entity2->HasComponent<TagComponent>() && entity2->GetComponent<TagComponent>()->GetTag() == "player")
		if (entity1->HasComponent<TagComponent>() && entity1->GetComponent<TagComponent>()->GetTag() == "enemy")
		{
			auto playerTransformComponent = entity2->GetComponent<TransformComponent>();
			auto playerPhysicsComponent = entity2->GetComponent<PhysicsComponent>();
			auto platformPhysicsComponent = entity1->GetComponent<PhysicsComponent>();

			// resolve interpenetration
			playerTransformComponent->SetPosition(playerTransformComponent->GetPosition() - glm::vec3(contactNormal * penetration, 0.0f));
		}

	// player/wall collision
	if (entity1->HasComponent<TagComponent>() && entity1->GetComponent<TagComponent>()->GetTag() == "player")
		if (entity2->HasComponent<TagComponent>() && entity2->GetComponent<TagComponent>()->GetTag() == "wall")
		{

		}

	if(entity2->HasComponent<TagComponent>() && entity2->GetComponent<TagComponent>()->GetTag() == "player")
		if (entity1->HasComponent<TagComponent>() && entity1->GetComponent<TagComponent>()->GetTag() == "wall")
		{

		}
}
