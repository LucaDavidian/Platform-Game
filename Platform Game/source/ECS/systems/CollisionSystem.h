#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include <glm/glm.hpp>

struct CollisionEvent;
class Entity;

class CollisionSystem
{
private:
	struct Collider
	{
		glm::vec2 position;
		glm::vec2 dimensions;
	};
public:
	static CollisionSystem &GetInstance() 
	{ 
		static CollisionSystem instance; 
		return instance; 
	}

	void Update(float dt);

	struct HitResult {

		Entity *entityHit;

	};

	//static HitResult RayCast(glm::vec2 startPosition, glm::vec2 direction, float lenght, const std::string tag)
	//{
	//	for (Entity *entity : Scene::GetInstance().GetEntities())  // #TODO: spatial partitioning optimization
	//	{
	//		if (entity->HasComponent<CollisionComponent>())
	//		{
	//			CollisionComponent *collisionComponent = entity->GetComponent<CollisionComponent>();


	//		}
	//	}


	//	return HitResult{};
	//}

private:
	CollisionSystem() = default;

	bool SAT(Collider c1, Collider c2, glm::vec2 &contactPoint, glm::vec2 &contactNormal, float &penetration);
	void ResolveCollision(Entity *entity1, Entity *entity2, glm::vec2 collisionPoint, glm::vec2 collisionNormal, float penetration);

	bool mPlayerOnPlatform;
};

#endif // COLLISIONSYSTEM_H
