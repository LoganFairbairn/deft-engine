#pragma once
#include <vector>
#include "UIDropDown.h"
#include "UISystem.h"

namespace Deft
{
	class UIDropDownMenu
	{
	public:
		UIDropDownMenu(UISystem* system, Vec2 position);
		~UIDropDownMenu() {};

		void Draw(Renderer* renderer);

		void AddCategory(const std::string& categoryName);
		void AddOption(const std::string& categoryName, const std::string& optionName);

		void SetPosition(Vec2 position);
		void SetBackgroundColor(ColorRGBA8 color) { m_BackgroundColor = color; }
		void SetSize(Vec2 size) { m_Size = size; }
		void SetDepth(float depth) { m_Depth = depth; }

		float GetMenuHeight() { return m_Size.y; }

	private:
		UISystem* m_UISystem;
		std::vector<UIDropDown*> m_DropDowns;

		Vec2 m_Position;
		Vec2 m_Size;
		ColorRGBA8 m_BackgroundColor;
		float m_Depth;
	};
}