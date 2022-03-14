#include "MenuGameState.h"
#include "../ECS/Scene.h"
#include "../Lua bindings/LuaVM.h"
#include "../logger/Logger.h"
#include "Game.h"

// #TODO_use_reflection
#include "../GUI/GUIPanel.h"
#include "../GUI/GUILabel.h"
#include "../GUI/GUIButton.h"
#include "../GUI/GUIManager.h"

void MenuGameState::OnEntry(Game *game)
{
	// create a Lua environment to load level from script 
	LuaVM script;

	if (!script.LoadFile("scripts/menu.lua"))
	{
		Logger::Instance().Log("error loading menu script", Logger::Level::LOG_ERROR);
		return;
	}

	if (!script.Execute())
	{
		Logger::Instance().Log("error executing menu script", Logger::Level::LOG_ERROR);
		return;
	}

	// load level 
	LuaObject levelTable = script.GetGlobal("menu");

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

			// iterate entity's components
			while (componentsTable.HasNext())
			{
				auto componentPair = componentsTable.GetNext();
				auto componentTypeDesc = Reflect::Resolve(componentPair.first.Cast<std::string>());
				auto component = componentTypeDesc->GetConstructor<LuaObject>()->NewInstance(componentPair.second);  // create component
				component.GetType()->GetMemberFunction("add_to_entity")->Invoke(Reflect::AnyRef(), component, Reflect::AnyRef(*entity));  // add component to entity
			}

			entity->Init();  // initialize entity --> initialize entity's components
			mEntities.InsertLast(entity);

			if (entityName == "camera")
				Scene::GetInstance().SetCamera(entity);
		}
	}

	// #TODO_GUI add menu screen + characters doing things + play and options buttons 

	// menu GUI
	GUIPanel *menuPanel = new GUIPanel(glm::vec2(0.0f, 0.0f), glm::vec2(0.8f, 0.8f), true);
	menuPanel->SetColor(0.0f, 0.5f, 1.0f);
	menuPanel->SetAchorPoint(GUIComponent::AnchorPoint::CENTER, GUIComponent::AnchorPoint::CENTER);
	menuPanel->SetRelativePosition(0.0f, 0.0f);

	GUILabel *title = new GUILabel(glm::vec2(), glm::vec2(0.5f, 0.2f), "Cave Adventure", "arial.ttf", 40);
	title->SetAchorPoint(GUIComponent::AnchorPoint::CENTER, GUIComponent::AnchorPoint::CENTER);
	title->SetRelativePosition(0.0f, 0.3f);

	GUIButton *playButton = new GUIButton(glm::vec2(), glm::vec2());
	playButton->SetRelativeDimensions(3.0f / 20.0f, 2.0f / 20.0f);
	playButton->SetAchorPoint(GUIComponent::AnchorPoint::CENTER, GUIComponent::AnchorPoint::CENTER);
	playButton->SetRelativePosition(0.0f, -0.1f);
	playButton->SetText("play");
	playButton->AddButtonClickedListener([game](const std::string&) { game->ChangeState((GameState*)&GameState::mPlayGameState); });

	GUIButton *quitButton = new GUIButton(glm::vec2(), glm::vec2());
	quitButton->SetRelativeDimensions(3.0f / 20.0f, 2.0f / 20.0f);
	quitButton->SetAchorPoint(GUIComponent::AnchorPoint::CENTER, GUIComponent::AnchorPoint::CENTER);
	quitButton->SetRelativePosition(0.0f, -0.3f);
	quitButton->SetText("quit");
	quitButton->AddButtonClickedListener([game](const std::string&) { game->Quit(); });

	menuPanel->AddGuiComponent(title);
	menuPanel->AddGuiComponent(playButton);
	menuPanel->AddGuiComponent(quitButton);

	GUIManager::GetInstance().AddGUIPanel(menuPanel);

	mGUIComponents.InsertLast(menuPanel);
}

void MenuGameState::OnExit(Game *game)
{
	for (auto entity : mEntities)
		if (Scene::GetInstance().HasEntity(entity))
			entity->Destroy();

	for (auto GUIComponent : mGUIComponents)
		GUIManager::GetInstance().RemoveGUIPanel(GUIComponent);
}

void MenuGameState::OnInput(Game *game, InputEvent &inputEvent)
{

}





