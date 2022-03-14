#include "SpriteAnimationSystem.h"
#include "../Scene.h"
#include "../components/SpriteComponent.h"
#include "../components/SpriteAnimationComponent.h"

void SpriteAnimationSystem::Update(float dt)
{
	for (Entity *entity : Scene::GetInstance().GetEntities())
		if (entity->HasComponents<SpriteComponent, SpriteAnimationComponent>())
		{
			SpriteComponent *spriteComponent = entity->GetComponent<SpriteComponent>();
			SpriteAnimationComponent *spriteAnimationComponent = entity->GetComponent<SpriteAnimationComponent>();

			if (spriteAnimationComponent->Update(dt))
				spriteComponent->SetTile(spriteAnimationComponent->GetCurrentTile());
		}
}