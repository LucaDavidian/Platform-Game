#include "DamageSystem.h"
#include "../Scene.h"
#include "../Entity.h"
#include "../components/LivesComponent.h"
#include "../components/TagComponent.h"
#include "../../event system/EventBus.hpp"
#include "../../event system/Event.hpp"
#include "../../logger/Logger.h"

void DamageSystem::OnCollision(CollisionEvent &event)
{
	if (event.entity1->HasComponent<TagComponent>() && event.entity1->GetComponent<TagComponent>()->GetTag() == "player" && event.entity2->HasComponent<TagComponent>() && event.entity2->GetComponent<TagComponent>()->GetTag() == "enemy")
	{
		if (event.entity1->HasComponent<LivesComponent>())
		{
			auto livesComponent = event.entity1->GetComponent<LivesComponent>();
			livesComponent->DecreaseNumLives();

			Logger::Instance().Log("player receives damage", Logger::Level::LOG_MESSAGE);

			if (livesComponent->GetNumLives() == 0)
				gEventBus.TriggerEvent<NoLivesLeftEvent>();
		}

		event.entity1->GetEventBus().TriggerEvent<DamageEvent>(event.entity1, event.contactNormal);
	}
	else if (event.entity1->HasComponent<TagComponent>() && event.entity1->GetComponent<TagComponent>()->GetTag() == "enemy" && event.entity2->HasComponent<TagComponent>() && event.entity2->GetComponent<TagComponent>()->GetTag() == "player")
	{
		if (event.entity2->HasComponent<LivesComponent>())
		{
			auto livesComponent = event.entity2->GetComponent<LivesComponent>();
			livesComponent->DecreaseNumLives();

			Logger::Instance().Log("player receives damage", Logger::Level::LOG_MESSAGE);

			if (livesComponent->GetNumLives() == 0)
				gEventBus.TriggerEvent<NoLivesLeftEvent>();
		}

		event.entity2->GetEventBus().TriggerEvent<DamageEvent>(event.entity2, -event.contactNormal);
	}

	//event.entity2->GetEventBus().TriggerEvent<EnemyDeadEvent>();
	//event.entity1->GetEventBus().TriggerEvent<EnemyDeadEvent>();
}

DamageSystem::DamageSystem()
{
	gEventBus.SubscribeToEvent<CollisionEvent>(*this, &DamageSystem::OnCollision);
}
