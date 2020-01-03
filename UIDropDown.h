#pragma once
#include "UIObject.h"
#include "UISystem.h"
#include "UIButton.h"

namespace Deft
{
	class UISystem;

	class UIDropDown : public UIButton
	{
	public:
		UIDropDown(UISystem* system);
		~UIDropDown() {};

		void Update(Vec2 mousePosition);
		void Draw(Renderer* renderer);

		void AddOption(const std::string& optionName);

	private:
		UISystem* m_UISystem;
		std::vector<UIButton*> m_Options;

		bool m_ShowOptions;
	};
}