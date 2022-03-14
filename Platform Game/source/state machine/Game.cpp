#include "Game.h"
#include "GameState.h"
#include <SDL2/SDL.h>
#include "../GraphicsManager.h"
#include "../Input/InputManager.h"
#include "../timer/Timer.h"
#include "../ECS/Scene.h"
#include "../ECS/systems/RenderSystem.h"
#include "../ECS/systems/SpriteAnimationSystem.h"
#include "../ECS/systems/TransformAnimationSystem.h"
#include "../ECS/systems/PhysicsSystem.h"
#include "../ECS/systems/CollisionSystem.h"
#include "../ECS/systems/MoveToSystem.h"
#include "../ECS/systems/DamageSystem.h"
#include "../ECS/systems/StateMachineSystem.hpp"
#include "../ECS/systems/InputSystem.h"
#include "../event system/Event.hpp"
#include "../event system/EventBus.hpp"

Game::Game()
{
	mInputConnection = InputManager::GetInstance().RegisterInputEvent(*this, &Game::OnInput);
}

Game::~Game()
{
	mInputConnection.Disconnect();

	SDL_Quit();
}

void Game::Initialize() 
{
	GraphicsManager::GetInstance().Initialize(); 

	Timer::GlobalTimer().Reset();   // init global timer (game loop)
	Timer::GlobalTimer().Start();

	PushState((GameState*)&GameState::mMenuGameState);  // initial transition
}

void Game::PushState(GameState *gameState)
{
	if (!mGameStates.Empty() && gameState == mGameStates.Top())
		return;

	// push and enter new state
	mGameStates.Push(gameState);
	mGameStates.Top()->OnEntry(this);
}

void Game::ChangeState(GameState *gameState)
{
	if (!mGameStates.Empty() && gameState == mGameStates.Top())
		return;

	// exit and pop old state if present
	if (!mGameStates.Empty())
	{
		mGameStates.Top()->OnExit(this);
		mGameStates.Pop();
	}
	
	// push and enter new state
	mGameStates.Push(gameState);
	mGameStates.Top()->OnEntry(this);
}

void Game::PopState()
{
	if (mGameStates.Empty())
		return;

	// exit and pop current state
	mGameStates.Top()->OnExit(this);
	mGameStates.Pop();
}

void Game::Run()
{
	Setup();

	SDL_Event event;
	while (!mQuit)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			switch (event.type)
			{
				case SDL_QUIT:
					mQuit = true;
					break;

				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							mQuit = true;
							break;

						case SDLK_F1:
							mDebug = !mDebug;
							break;
					}

					break;
			}
		}
		
		Loop();
	}
}

void Game::Loop()
{
	//static float lag = 0.0f;
	//static float updateFrameTime = 1.0f / 60.0f;

	Timer::GlobalTimer().Tick();
	float frameTimeMs = Timer::GlobalTimer().GetDeltaTimeMilliseconds();

	// lag += frameTimeMs;

	ProcessInput();

	Update(frameTimeMs);  // variable update delta time

	//while (lag <= updateFrameTime)   // fixed update delta time (real time for an update must be less than updateFrameTime)
	//{
	//	Update(updateFrameTime);
	//	lag -= updateFrameTime;
	//}

	Render();
}

void Game::Setup()
{
	DamageSystem::GetInstance();
}

void Game::ProcessInput()
{
	InputManager::GetInstance().DispatchInput();  // input events are sent to Game that dispatches them to currently active state
}

void Game::Update(float frameTimeMs)  // #TODO_update_active_state (pause others)
{
	float FPS = 1000.0 / frameTimeMs;
	GraphicsManager::GetInstance().SetWindowsTitle("Game - FPS: " + std::to_string(FPS));

	// update systems
	//TransformAnimationSystem::GetInstance().Update(frameTimeMs);
	SpriteAnimationSystem::GetInstance().Update(frameTimeMs);
	MoveToSystem::GetInstance().Update(frameTimeMs);
	PhysicsSystem::GetInstance().Update(frameTimeMs);
	CollisionSystem::GetInstance().Update(frameTimeMs);
	StateMachineSystem::GetInstance().Update(frameTimeMs);
	InputSystem::GetInstance().Update();

	// update scene (kill destroyed entities)
	Scene::GetInstance().Update();
}

void Game::Render()
{
	// clear screen
	GraphicsManager::GetInstance().Clear();

	// render scene
	RenderSystem::GetInstance().Render();

	if (mDebug)
		RenderSystem::GetInstance().RenderDebug();

	// present screen
	GraphicsManager::GetInstance().Present();
}

void Game::OnInput(InputEvent &inputEvent)
{
	//mGameStates.Top()->OnInput(this, inputEvent);
}

void Game::Restart()
{
	while (!mGameStates.Empty())
		PopState();

	ChangeState((GameState*)&GameState::mMenuGameState);
}
