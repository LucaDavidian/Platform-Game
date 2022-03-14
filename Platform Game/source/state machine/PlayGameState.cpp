#include "PlayGameState.h"
#include "Game.h"
#include "../ECS/Scene.h"
#include "../ECS/Entity.h"
#include "../ECS/components/InputComponent.h"
#include "../ECS/systems/RenderSystem.h"
#include "../Lua bindings/LuaVM.h"
#include "../logger/Logger.h"

// #TODO_use_reflection
#include "../GUI/GUIPanel.h"
#include "../GUI/GUILabel.h"
#include "../GUI/GUIImage.h"
#include "../GUI/GUIButton.h"
#include "../GUI/GUIManager.h"
#include "../GUI/GUILifeBar.h"

void PlayGameState::OnEntry(Game *game)
{
	// 	GUILabel *pointsLabel = new GUILabel(glm::vec2(0.2f, 0.0f), glm::vec2(0.1f, 0.05f), "points:", "arial.ttf", 25);
	// 	pointsLabel->SetColor(glm::vec3(0.0f, 0.0f, 0.0f));

	mLifeBar = new GUILifeBar(glm::vec2(0.03f, 0.01f), glm::vec2(0.2f, 0.05));  // create GUI first, life component send event to set num hearts
	mLifeBar->SetAchorPoint(GUIComponent::AnchorPoint::TOP_LEFT, GUIComponent::AnchorPoint::TOP_LEFT);

	//GUIManager::GetInstance().AddGUIPanel(pointsLabel);
	GUIManager::GetInstance().AddGUIPanel(mLifeBar);

	// create a Lua environment to load level from script 
	LuaVM script;

	if (!script.LoadFile("scripts/level.lua"))
	{
		Logger::Instance().Log("error loading level script", Logger::Level::LOG_ERROR);
		return;
	}

	if (!script.Execute())
	{
		Logger::Instance().Log("error executing level script", Logger::Level::LOG_ERROR);
		return;
	}

	// load level 
	LuaObject levelTable = script.GetGlobal("level");
	
	LuaObject entitiesTable = levelTable.Get("entities");

	// iterate entities
	if (entitiesTable)
	{
		while (entitiesTable.HasNext())
		{
			auto entityPair = entitiesTable.GetNext();

			std::string entityName = entityPair.first;
			LuaObject componentsTable = entityPair.second.Get("components");

			Entity *entity = Scene::GetInstance().AddEntity();

			bool hasFSM = false;

			// iterate entity's components
			while (componentsTable.HasNext())
			{
				auto componentPair = componentsTable.GetNext();
				auto componentTypeDesc = Reflect::Resolve(componentPair.first.Cast<std::string>());

				if (componentPair.first.Cast<std::string>() == "FSM")
					hasFSM = true;

				auto component = componentTypeDesc->GetConstructor<LuaObject>()->NewInstance(componentPair.second);  // create component
				component.GetType()->GetMemberFunction("add_to_entity")->Invoke(Reflect::AnyRef(), component, Reflect::AnyRef(*entity));  // add component to entity
			}

			if (entityName == "player" && !hasFSM)
				int i = 10;

			entity->Init();  // initialize entity --> initialize entity's components
			mEntities.InsertLast(entity);

			if (entityName == "player")
				Scene::GetInstance().SetCamera(entity);
		}
	}
}

void PlayGameState::OnExit(Game *game)
{
	for (auto entity : mEntities)
		if (Scene::GetInstance().HasEntity(entity))
			entity->Destroy();

	GUIManager::GetInstance().RemoveGUIPanel(mLifeBar);
}





