#include "GameUI.h"
#include "Colors.h"

namespace Deft
{
	//Initializes all of the games UI.
	void GameUI::Initialize(UISystem* system, Renderer* renderer)
	{
		//Store pointers for the UISystem and the Renderer.
		m_UISystem = system;
		m_Renderer = renderer;

		InitializeMainMenu();
		InitializeSettings();
		InitializeEditor();

		//Hide groups.
		m_UISystem->HideGroup(UIGroup::UI_GROUP_SETTINGS);
		m_UISystem->HideGroup(UIGroup::UI_GROUP_EDITOR);
	}

	//Update the UI.
	void GameUI::Update(int fps)
	{
		unsigned int verticesRendering = m_Renderer->GetVerticesRendering();
		unsigned int indicesRendering = m_Renderer->GetIndicesRendering();

		m_PerformanceStats->SetText("FPS: " + std::to_string((int)fps) +
			"\nVertices Rendering: " + std::to_string(verticesRendering) +
			"\nIndices Rendering: " + std::to_string(indicesRendering));

		float x = 5.0f;
		float y = m_Renderer->GetCameraHeight() - m_PerformanceStats->GetSize().y - 5.0f;
		m_PerformanceStats->SetPosition(Vec2(x, y));
	}

	//Initializes UI for the main menu.
	void GameUI::InitializeMainMenu()
	{
		UIText* text = m_UISystem->AddText(Vec2(0.0f, 0.0f), "SHIFTING SANDS");
		text->SetDescription("This is the selected RPG universe.");
		text->SetFont("VTCSwitchbladeRomance");

		float drawX = m_Renderer->GetCameraWidth() * 0.5f - (text->GetSize().x * 0.5f);
		float drawY = m_Renderer->GetCameraHeight() - 5.0f - text->GetSize().y - 10.0f;
		text->SetPosition(Vec2(drawX, drawY));

		m_UIButtonMenu.SetMenuColor(COLOR_UI_GREY);
		m_UIButtonMenu.SetMenuHighlightColor(COLOR_UI_BLUE);
		m_UIButtonMenu.SetMenuFont("Vidaloka");

		m_UIButtonMenu.AddMenuOption(m_UISystem, "JOIN SERVER");
		m_UIButtonMenu.AddMenuOption(m_UISystem, "CREATE SERVER");
		m_UIButtonMenu.AddMenuOption(m_UISystem, "GAME EDITOR");
		m_UIButtonMenu.AddMenuOption(m_UISystem, "CHARACTERS");
		m_UIButtonMenu.AddMenuOption(m_UISystem, "SETTINGS");
		m_UIButtonMenu.AddMenuOption(m_UISystem, "CREDITS");
		m_UIButtonMenu.AddMenuOption(m_UISystem, "QUIT GAME");

		m_UIButtonMenu.SetMenuOptionDescription(0, "Joins a server.");
		m_UIButtonMenu.SetMenuOptionDescription(1, "Creates a multiplayer server.");
		m_UIButtonMenu.SetMenuOptionDescription(2, "Loads the game's editor.");
		m_UIButtonMenu.SetMenuOptionDescription(3, "Opens your character profiles.");
		m_UIButtonMenu.SetMenuOptionDescription(4, "Opens game settings.");
		m_UIButtonMenu.SetMenuOptionDescription(5, "Shows the credits for this game and engine.");
		m_UIButtonMenu.SetMenuOptionDescription(6, "Closes the games application.");

		drawY -= 30.0f;
		m_UIButtonMenu.SetMenuPosition(Vec2(10.0f, drawY));

		//////////////////////////////////////////////////////////////////////
		//Test Stuff...

		Material closeButton = ResourceManager::GetMaterial("CloseButton");
		Material closeButtonOver = ResourceManager::GetMaterial("CloseButtonOver");

		UIButton* button = m_UISystem->AddButton(Vec2(500.0f, 200.0f));
		button->SetGraphics(closeButton, closeButtonOver);

		//UIDropDown* dropDown = m_UISystem->AddDropDown(Vec2(200.0f, 200.0f), "DropDown");
		//dropDown->AddOption("");
	}

	//Initializes the UI for the game settings.
	void GameUI::InitializeSettings()
	{
		UIText* text = m_UISystem->AddText(Vec2(0.0f, 0.0f), "SETTINGS");
		text->SetGroup(UIGroup::UI_GROUP_SETTINGS);
	}

	//Initialize the UI for the game's editor.
	void GameUI::InitializeEditor()
	{
		//Create the drop down menu.
		m_UIDropDownMenu = std::make_unique<UIDropDownMenu>(m_UISystem, Vec2(0.0f, 0.0f));

		m_UIDropDownMenu->AddCategory("File");
		m_UIDropDownMenu->AddCategory("Edit");
		m_UIDropDownMenu->AddCategory("Map");
		m_UIDropDownMenu->AddCategory("Editor");

		m_UIDropDownMenu->SetPosition(Vec2(5.0f, 300.0f));

		//m_UIDropDownMenu->AddOption("File", "Exit");

		//Performance Text
		m_PerformanceStats = m_UISystem->AddText(Vec2(0.0f, 0.0f), "FPS");

		float x = 5.0f;
		float y = m_Renderer->GetCameraHeight() - m_UIDropDownMenu->GetMenuHeight() - m_PerformanceStats->GetSize().y - 5.0f;

		m_PerformanceStats->SetPosition(Vec2(x, y));
		m_PerformanceStats->SetColor(ColorRGBA8(0, 255, 0, 255));
		m_PerformanceStats->SetGroup(UIGroup::UI_GROUP_EDITOR);

		//Create the window.
		UIWindow* window = m_UISystem->AddWindow(Vec2(100.0f, 100.0f), Vec2(400.0f, 400.0f), "Entity Manager");
		window->SetVisible(false);
		float windowHeight = window->GetSize().y;

		UIText* text = m_UISystem->AddText(Vec2(5.0f, 5.0f), "This is a text!");

		//Add objects to the window.
		window->AddObject(text);

		//Window 2
		window = m_UISystem->AddWindow(Vec2(600.0f, 100.0f), Vec2(400.0f, 400.0f), "Entity List");
		window->SetVisible(false);

		//Window 3
		window = m_UISystem->AddWindow(Vec2(600.0f, 400.0f), Vec2(0.0f, 0.0f), "Tile Editor");
		window->SetVisible(false);
	}
}