#include "UIButtonMenu.h"
#include "Colors.h"

namespace Deft
{
	UIButtonMenu::UIButtonMenu()
	{
		m_FontName = ResourceManager::GetDefaultFontName();
		m_Color = ColorRGBA8(255, 255, 255, 255);
		m_Spacing = 2.0f;
		m_Width = 500.0f;

		m_Color = COLOR_UI_GREY;
		m_HighlightColor = COLOR_UI_BLUE;
	}

	//Adds a menu option to the UI.
	void UIButtonMenu::AddMenuOption(UISystem* UISystem, const std::string& optionName)
	{
		//Add the menu option to the UI.
		UIButton* button = UISystem->AddButton(Vec2(0.0f, 0.0f));

		m_MenuOptions.push_back(button);

		//Set the button text.
		button->SetText(optionName);
		button->SetTextAlignment(TextAlignment::LEFT);

		//Set button background.
		button->DrawBackground(true);
		button->SetBackgroundPadding(5.0f);
		button->SetBGNormalColor(m_Color);
		button->SetBGHighlightColor(m_HighlightColor);

		float buttonHeight = button->GetFont()->MeasureText(button->GetText().c_str()).y + (button->GetBGPadding() * 2);
		button->SetCustomBGSize(Vec2(m_Width, buttonHeight));

		//Set the position for the menu to reposition all of the menu options.
		SetMenuPosition(m_Position);
	}

	//Makes all of the objects in the menu visible or not visible.
	void UIButtonMenu::SetVisible(bool visible)
	{
		for (unsigned int i = 0; i < m_MenuOptions.size(); i++)
		{
			m_MenuOptions[i]->SetVisible(visible);
			m_MenuOptions[i]->SetMouseIsOver(false);
		}
	}

	//Clears the text for all menu options.
	void UIButtonMenu::ClearMenu()
	{
		//Set the color for all of the text buttons.
		for (unsigned int i = 0; i < m_MenuOptions.size(); i++)
		{
			m_MenuOptions[i]->SetText("");
		}
	}

	void UIButtonMenu::SetMenuOptionDescription(unsigned int optionIndex, const std::string& optionDescription)
	{
		m_MenuOptions[optionIndex]->SetDescription(optionDescription);
	}

	//Sets the position for the menu.
	void UIButtonMenu::SetMenuPosition(Vec2 position)
	{
		m_Position = position;

		SpriteFont* font = ResourceManager::GetFont(m_FontName);
		float fontHeight = font->GetFontHeight();

		//Set the position for all of the menu options.
		for (unsigned int i = 0; i < m_MenuOptions.size(); i++)
		{
			float x = m_Position.x;
			float y = m_Position.y - fontHeight - (i * fontHeight) - (i * m_Spacing - m_Spacing);
			m_MenuOptions[i]->SetPosition(Vec2(x, y));
		}
	}

	//Sets the font for the menu options.
	void UIButtonMenu::SetMenuFont(const std::string& fontName)
	{
		m_FontName = fontName;

		//Set the font for all of the text buttons.
		for (unsigned int i = 0; i < m_MenuOptions.size(); i++)
		{
			m_MenuOptions[i]->SetFont(fontName);
		}
	}

	//Sets the color for the menu options.
	void UIButtonMenu::SetMenuColor(ColorRGBA8 color)
	{
		m_Color = color;

		//Set the color for all of the text buttons.
		for (unsigned int i = 0; i < m_MenuOptions.size(); i++)
		{
			m_MenuOptions[i]->SetColor(color);
		}
	}

	//Sets the highlight color for the menu.
	void UIButtonMenu::SetMenuHighlightColor(ColorRGBA8 color)
	{
		m_HighlightColor = color;

		//Set the color for all of the text buttons.
		for (unsigned int i = 0; i < m_MenuOptions.size(); i++)
		{
			m_MenuOptions[i]->SetBGHighlightColor(color);
		}
	}

	//Gets the size of the font.
	float UIButtonMenu::GetFontHeight()
	{
		SpriteFont* font = ResourceManager::GetFont(m_FontName);
		return font->GetFontHeight();
	}
}