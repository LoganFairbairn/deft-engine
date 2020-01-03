#pragma once
#include "UISystem.h"
#include "UIButton.h"
#include "ResourceManager.h"

namespace Deft
{
	//This is a menu comprised of UITextButtons.
	class UIButtonMenu
	{
	public:
		UIButtonMenu();
		~UIButtonMenu() {};

		void AddMenuOption(UISystem* UISystem, const std::string& optionName);
		void SetVisible(bool visible);
		void ClearMenu();
		
		//Setters.
		void SetMenuOptionDescription(unsigned int optionIndex, const std::string& optionDescription);
		void SetMenuPosition(Vec2 position);
		void SetMenuFont(const std::string& fontName);
		void SetMenuColor(ColorRGBA8 color);
		void SetMenuHighlightColor(ColorRGBA8 color);
		void SetSpacing(float spacing) { m_Spacing = spacing; }

		//Getters.
		float GetFontHeight();

	private:
		std::vector<UIButton*> m_MenuOptions;

		Vec2 m_Position;
		float m_Spacing;				//Space between buttons.
		float m_Width;
		std::string m_FontName;
		ColorRGBA8 m_Color;
		ColorRGBA8 m_HighlightColor;
	};
}