#include "MainSystem.h"

//These are for testing entity creation, remove later.
#include "TransformComponent.h"
#include "MaterialComponent.h"
#include "KeyboardComponent.h"
#include "AnimationComponent.h"
#include "Material.h"

namespace Deft
{
	void MainSystem::Start()
	{
		Initialize();
		GameLoop();
	}

	//Performs actions based on game events that are recieved.
	void MainSystem::HandleGameEvent(GameEvents gameEvent)
	{
		//Opens the game editor.
		if (gameEvent == GameEvents::OPEN_GAME_EDITOR)
		{
			SetGameState(GameState::GAMESTATE_EDITOR);
		}

		//Quits the game.
		if (gameEvent == GameEvents::QUIT_GAME)
		{
			SDL_Quit();
			exit(69);
		}
	}

	//Initializes anything needed for the engine.
	void MainSystem::Initialize()
	{
		//Initialize SDL.
		SDL_Init(SDL_INIT_EVERYTHING);

		//Create a renderer to draw the game.
		m_Renderer = std::make_unique<Renderer>();

		//Create an MessageBus.
		m_MessageBus = std::make_unique<MessageBus>();

		//Create engine System and give them a pointer to the MessageBus.
		m_InputSystem = std::make_unique<InputSystem>(m_MessageBus.get());
		m_AudioSystem = std::make_unique<AudioSystem>(m_MessageBus.get());
		m_UISystem = std::make_unique<UISystem>(m_MessageBus.get());

		//Initialize ResourceManager.
		ResourceManager::Initialize(m_MessageBus.get());

		//Define game System for the MessageBus.
		m_MessageBus->AddSystem(Systems::SYSTEM_MAIN, (System*)this);
		m_MessageBus->AddSystem(Systems::SYSTEM_UI, (System*)m_UISystem.get());
		m_MessageBus->AddSystem(Systems::SYSTEM_INPUT, (System*)m_InputSystem.get());
		m_MessageBus->AddSystem(Systems::SYSTEM_AUDIO, (System*)m_AudioSystem.get());

		//Make a loader (used for loading assets in managable chunks).
		m_Loader = std::make_unique<Loader>();		

		LoadRequiredAssets();		//Loads required assets, such as shaders and default fonts.
		InitializeUI();				//Initialize UI.

		m_MessageBus->LogMessage("Engine Version: " + std::to_string(ENGINE_VERSION));
	}

	//Loads default and loading screen assets.
	void MainSystem::LoadRequiredAssets()
	{
		//Load shaders first.
		ResourceManager::LoadShader("Default");
		ResourceManager::LoadShader("Primitive");
		ResourceManager::LoadShader("Red");
		ResourceManager::SetDefaultShader("Default");

		//Initialize any shader programs.
		InitializeShaders();

		//Load fonts.
		ResourceManager::LoadFont("Default", "Resources/Fonts/SourceCodeProBold.ttf", 12);
		ResourceManager::LoadFont("Default_32_Bold", "Resources/Fonts/SourceCodeProBold.ttf", 36);
		ResourceManager::LoadFont("Font_MessageBox", "Resources/Fonts/SourceCodeProBold.ttf", 12);
		ResourceManager::LoadFont("Font_Loading", "Resources/Fonts/SourceCodeProBold.ttf", 20);
		ResourceManager::LoadFont("Vidaloka", "Resources/Fonts/VidalokaRegular.ttf", 26);
		ResourceManager::LoadFont("VTCSwitchbladeRomance", "Resources/Fonts/VTCSwitchbladeRomance.ttf", 32);
		ResourceManager::SetDefaultFont("Default");

		//Load textures.
		ResourceManager::LoadTexture("EngineLogo", "Resources/Logos/DeftEngineLogoWhite.png");
		ResourceManager::LoadTexture("Cursor_Normal", "Resources/Textures/Cursor_Normal.png");
		ResourceManager::LoadTexture("CloseButton", "Resources/Required/CloseButton.png");
		ResourceManager::LoadTexture("CloseButtonOver", "Resources/Required/CloseButtonOver.png");

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//TESTING REMOVE LATER!!!!!!!!!!
		//Create a bunch of blank entities for testing.
		ResourceManager::LoadTexture("Link", "Resources/Textures/Link.png");


		//Make a material.
		ResourceManager::MakeMaterial("RedLink", "Link", "Red");

		//Get the material.
		Material linkMat = ResourceManager::GetMaterial("Link");
		Material redLinkMat = ResourceManager::GetMaterial("RedLink");

		float posX = 5.0f;
		float posY = 5.0f;

		for (unsigned int i = 0; i < 50; i++)
		{
			//Create an entity, then add components.
			Entity* entity = m_EntityManager.AddEntity("Entity" + std::to_string(i));
			entity->AddComponent<TransformComponent>(ComponentType::TransformComponent);
			entity->AddComponent<MaterialComponent>(ComponentType::MaterialComponent);

			//Edit components.
			entity->GetComponent<TransformComponent>(ComponentType::TransformComponent)->SetPosition(Vec2(posX, posY));
			entity->GetComponent<TransformComponent>(ComponentType::TransformComponent)->SetScale(Vec2(1.0f, 1.0f));
			entity->GetComponent<MaterialComponent>(ComponentType::MaterialComponent)->SetDepth(0.5f);

			if (i >= 25)
			{
				entity->GetComponent<MaterialComponent>(ComponentType::MaterialComponent)->SetMaterial(linkMat);
			}
			
			else
			{
				entity->GetComponent<MaterialComponent>(ComponentType::MaterialComponent)->SetMaterial(redLinkMat);
			}

			posX += linkMat.texture.width + 5.0f;

			if (posX > m_Renderer->GetCameraWidth())
			{
				posX = 5.0f;
				posY += linkMat.texture.width + 5.0f;
			}
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}

	//Initialize Shaders.
	void MainSystem::InitializeShaders()
	{
		Shader* shader = ResourceManager::GetShader("Default");
		shader->CompileShader("Resources/Shaders/DefaultShader.vert", "Resources/Shaders/DefaultShader.frag");
		shader->AddAttribute("in_VertexPosition");
		shader->AddAttribute("in_VertexColor");
		shader->AddAttribute("in_VertexUV");
		shader->LinkShaders();

		Shader* primitiveShader = ResourceManager::GetShader("Primitive");
		primitiveShader->CompileShader("Resources/Shaders/PrimitiveShader.vert", "Resources/Shaders/PrimitiveShader.frag");
		primitiveShader->AddAttribute("in_VertexPosition");
		primitiveShader->AddAttribute("in_VertexColor");
		primitiveShader->LinkShaders();

		Shader* redShader = ResourceManager::GetShader("Red");
		redShader->CompileShader("Resources/Shaders/RedShader.vert", "Resources/Shaders/RedShader.frag");
		redShader->AddAttribute("in_VertexPosition");
		redShader->AddAttribute("in_VertexColor");
		redShader->AddAttribute("in_VertexUV");
		redShader->LinkShaders();
	}

	void MainSystem::InitializeUI()
	{
		//Set the cursor.
		Material cursorMaterial = ResourceManager::GetMaterial("Cursor_Normal");
		m_UISystem->SetCursor(cursorMaterial, Vec2(0.0f, cursorMaterial.texture.height));

		//Initialize the UI.
		m_GameUI.Initialize(m_UISystem.get(), m_Renderer.get());
	}

	//This is the main game loop. This runs continously until an exit command has been given.
	void MainSystem::GameLoop()
	{
		m_FPSLimiter.SetMaxFPS(m_DesiredFPS);

		const int MAX_PHYSICS_STEPS = 6;							//How many frames can be simulated before the game is forced to render.
		const float MAX_DELTA_TIME = 1.0f;
		float m_DesiredFrameTime = 1000.0f / m_DesiredFPS;			//How much time you want your frames to take to render (this is based on your desired FPS).
		unsigned int previousTicks = SDL_GetTicks();				//Get the ticks just before the game loop starts so there is something in previous ticks.

		//Start by loading the game, then go to the main menu.
		m_PreviousGameState = GameState::GAMESTATE_MENU;
		SetGameState(GameState::GAMESTATE_LOADING);

		while (m_GameState != GameState::GAMESTATE_EXIT)
		{
			m_FPSLimiter.Begin();

			float newTicks = SDL_GetTicks();
			float frameTime = newTicks - previousTicks;

			previousTicks = newTicks;

			float totalDeltaTime = frameTime / m_DesiredFrameTime;

			int i = 0;
			while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS)
			{
				float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);

				m_GameUI.Update(m_FPS);

				//Update & draw the game based on the game state.	
				switch (m_GameState)
				{
					case GameState::GAMESTATE_LOADING:
					{
						GameStateLoading();
					}
					break;

					case GameState::GAMESTATE_MENU:
					{
						GameStateMenu();
					}
					break;

					case GameState::GAMESTATE_EDITOR:
					{
						GameStateEditing();
					}
					break;

					case GameState::GAMESTATE_PLAYING:
					{
						GameStatePlaying();
					}
					break;
				}

				totalDeltaTime -= deltaTime;
				i++;
			}

			//Calculate the frames per second.
			m_FPS = m_FPSLimiter.End();
		}
	}

	//Load / Unload assets.
	void MainSystem::GameStateLoading()
	{
		//Update.
		m_Loader->Load();
		m_Renderer->Update();

		//Draw.
		m_Renderer->ClearScreen();
		m_Renderer->StartBatching(RendererDrawMode::DRAW_MODE_UI);

		m_Loader->DrawLoading(m_Renderer.get());

		m_Renderer->EndBatching();
		m_Renderer->SwapWindowBuffer();

		//Calculate the loading time (in seconds).
		m_LoadingTime = (SDL_GetTicks() - m_LoadingStartTime) / 1000.0f;

		if (m_LoadingTime >= MIN_LOADING_TIME)
		{
			//Debug info.
			m_MessageBus->LogMessage("Loading Time: " + std::to_string(m_LoadingTime));
			ResourceManager::OutputResourceList();

			//After loading, go back to the previous game state.
			if (m_Loader->GetPercentCompleted() == 100.0f)
			{
				SetGameState(m_PreviousGameState);
			}
		}
	}

	//Draw the game menu and update menu UI.
	void MainSystem::GameStateMenu()
	{
		//Update.
		m_MessageBus->Update(m_FPS);
		m_Renderer->Update();
		m_InputSystem->Update();

		Vec2 mouseWorldPosition = m_Renderer->ScreenToWorldCoordinates(m_InputSystem->GetMousePosition());
		m_UISystem->Update(mouseWorldPosition);

		//Draw.
		m_Renderer->ClearScreen();

		m_Renderer->StartBatching(RendererDrawMode::DRAW_MODE_UI);
		m_UISystem->Draw(m_Renderer.get());
		m_Renderer->EndBatching();

		m_Renderer->StartBatching(RendererDrawMode::DRAW_MODE_UI_WINDOWS);
		m_UISystem->DrawCursor(m_Renderer.get());
		m_Renderer->EndBatching();

		m_Renderer->SwapWindowBuffer();
	}

	//Edit game content.
	void MainSystem::GameStateEditing()
	{
		//Update.
		m_Renderer->Update();
		m_MessageBus->Update(m_FPS);
		m_InputSystem->Update();
		m_GameUI.Update(m_FPS);

		Vec2 mouseWorldPosition = m_Renderer->ScreenToWorldCoordinates(m_InputSystem->GetMousePosition());
		m_UISystem->Update(mouseWorldPosition);

		//Draw.
		m_Renderer->ClearScreen();

		//Draw the game.
		m_Renderer->StartBatching(RendererDrawMode::DRAW_MODE_GAME);
		m_EntityManager.DrawEntities(m_Renderer.get());
		m_Renderer->EndBatching();

		//Draw UI.
		m_Renderer->StartBatching(RendererDrawMode::DRAW_MODE_UI);
		m_UISystem->Draw(m_Renderer.get());
		m_Renderer->EndBatching();

		//Draw UIWindows.
		m_Renderer->StartBatching(RendererDrawMode::DRAW_MODE_UI_WINDOWS);
		m_UISystem->DrawWindows(m_Renderer.get());
		m_UISystem->DrawCursor(m_Renderer.get());
		m_Renderer->EndBatching();

		m_Renderer->SwapWindowBuffer();
	}

	//Play the game.
	void MainSystem::GameStatePlaying()
	{

	}

	//Sets the game to the specified game state.
	void MainSystem::SetGameState(GameState gameState)
	{
		switch (gameState)
		{
			case GameState::GAMESTATE_LOADING:
			{
				m_GameState = GameState::GAMESTATE_LOADING;
				m_Renderer->SetClearColor(0.0f, 0.0f, 0.0f);
				m_LoadingTime = 0.0f;
				m_LoadingStartTime = SDL_GetTicks();
			}
			break;

			case GameState::GAMESTATE_EDITOR:
			{
				m_GameState = GameState::GAMESTATE_EDITOR;
				m_Renderer->SetClearColor(0.2f, 0.2f, 0.2f);

				//Toggle UI visibility.
				m_UISystem->HideGroup(UIGroup::UI_GROUP_MAIN);
				m_UISystem->HideGroup(UIGroup::UI_GROUP_SETTINGS);

				m_UISystem->ShowGroup(UIGroup::UI_GROUP_EDITOR);

				m_UISystem->ShowWindow("Entity Manager");
				m_UISystem->ShowWindow("Entity List");
			}
			break;

			case GameState::GAMESTATE_MENU:
			{
				m_GameState = GameState::GAMESTATE_MENU;
				m_Renderer->SetClearColor(0.2f, 0.2f, 0.2f);

				//Toggle UI visibility.
				m_UISystem->HideGroup(UIGroup::UI_GROUP_EDITOR);
				m_UISystem->HideGroup(UIGroup::UI_GROUP_SETTINGS);

				m_UISystem->ShowGroup(UIGroup::UI_GROUP_MAIN);

				m_UISystem->HideWindow("Entity Manager");
				m_UISystem->HideWindow("Entity List");
			}
			break;

			case GameState::GAMESTATE_PLAYING:
			{
				m_GameState = GameState::GAMESTATE_PLAYING;
				m_Renderer->SetClearColor(0.2f, 0.2f, 0.2f);
			}
			break;
		}
	}
}