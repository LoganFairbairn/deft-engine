#pragma once
#include <SDL/SDL.h>		//Include SDL.
#include <vector>
#include <algorithm>
#include <memory>

//Engine
#include "GameState.h"
#include "FPSLimiter.h"
#include "System.h"
#include "MessageBus.h"

//Entities
#include "EntityManager.h"

//Audio
#include "AudioSystem.h"

//Input
#include "InputSystem.h"

//Assets and Loading
#include "Loader.h"
#include "ResourceManager.h"

//User Interface
#include "UISystem.h"
#include "UIButtonMenu.h"
#include "GameUI.h"

//Editor
#include "Editor.h"

namespace Deft
{
	const static float ENGINE_VERSION = 0.05f;
	const float MIN_LOADING_TIME = 1.0f;		//Minimum loading time in seconds.

	//This is the main system, and the core of the engine.
	class MainSystem : public System
	{
	public:
		MainSystem() {};
		~MainSystem() {};

		void Start();
		void HandleGameEvent(GameEvents gameEvent);

	private:
		void Initialize();
		void LoadRequiredAssets();
		void InitializeShaders();
		void InitializeUI();

		void GameLoop();
		void GameStateLoading();
		void GameStateMenu();
		void GameStateEditing();
		void GameStatePlaying();

		void SetGameState(GameState gameState);

		//TODO: Move FPS stuff into the FPSLimiter object.
		FPSLimiter m_FPSLimiter;
		float m_FPS = 0.0f;
		float m_DesiredFPS = 60.0f;
		float m_DesiredFrameTime = 0.0f;
		
		//Game state
		GameState m_GameState;
		GameState m_PreviousGameState;

		//Message Bus
		std::unique_ptr<MessageBus> m_MessageBus;

		//UI
		std::unique_ptr<UISystem> m_UISystem;
		GameUI m_GameUI;
		UIText* m_PerformanceStats;

		//Renderer (handles all drawing).
		std::unique_ptr<Renderer> m_Renderer;

		//Input
		std::unique_ptr<InputSystem> m_InputSystem;

		//Audio
		std::unique_ptr<AudioSystem> m_AudioSystem;

		//Loading
		std::unique_ptr<Loader> m_Loader;
		float m_LoadingStartTime = 0.0f;
		float m_LoadingTime = 0.0f;

		//Entities
		EntityManager m_EntityManager;

		//Editor
		Editor m_Editor;
	};
}
