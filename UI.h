#pragma once
#include "UISystem.h"

namespace Deft
{
	//All menu UI is defined here.
	class MainMenu
	{
	public:
		MainMenu(UISystem* system);
		~MainMenu() {};

		void SetVisible(bool visible) { m_Visible = visible; }

	private:
		UISystem* m_UISystem;

		bool m_Visible = true;
	};
}