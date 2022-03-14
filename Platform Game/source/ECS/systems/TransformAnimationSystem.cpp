#include "TransformAnimationSystem.h"
#include "../Scene.h"
#include "../components/TransformComponent.h"
#include "../components/TransformAnimationComponent.h"
#include "../../event system/Event.hpp"

void TransformAnimationSystem::Update(float dt)
{
	for (Entity *entity : Scene::GetInstance().GetEntities())
	{
		if (entity->HasComponents<TransformComponent, TransformAnimationComponent>())
		{
			TransformComponent *transformComponent = entity->GetComponent<TransformComponent>();
			TransformAnimationComponent *transformAnimationComponent = entity->GetComponent<TransformAnimationComponent>();

			if (transformAnimationComponent->Update(dt))
			{
				transformComponent->SetPosition(transformAnimationComponent->GetCurrentPosition());
				transformComponent->SetOrientation(transformAnimationComponent->GetCurrentOrientation());
				transformComponent->SetScale(transformAnimationComponent->GetCurrentScale());
			}
			else
				entity->GetEventBus().TriggerEvent<TransformAnimationEndEvent>();
		}
	}
}