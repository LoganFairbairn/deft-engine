#pragma once
#include "Renderer.h"
#include "UISystem.h"
#include "UIButtonMenu.h"
#include "UIDropDownMenu.h"

namespace Deft
{
	//All of Game UI can be initialized or deleted using this object.
	class GameUI
	{
	public:
		GameUI() {};
		~GameUI() {};

		void Initialize(UISystem* system, Renderer* renderer);
		void Update(int fps);

	private:
		void InitializeMainMenu();
		void InitializeSettings();
		void InitializeEditor();

		UISystem* m_UISystem;	//UISystem used to edit UI objects.
		Renderer* m_Renderer;	//Renderer used to position UI properly.

		UIText* m_PerformanceStats;

		UIButtonMenu m_UIButtonMenu;
		std::unique_ptr<UIDropDownMenu> m_UIDropDownMenu;
	};
}