#include "UIDropdown.h"
#include "Colors.h"

namespace Deft
{
	UIDropDown::UIDropDown(UISystem* system)
	{
		m_UISystem = system;

		//Default values.
		m_Color = COLOR_UI_GREY;
		m_BGNormalColor = COLOR_UI_GREY;
		m_BGHighlightColor = COLOR_UI_BLUE;
		m_BGDisabledColor = COLOR_UI_DISABLED;

		SetText("DropDown");

		SetScale(Vec2(1.0f, 1.0f));
		SetDepth(-0.9f);

		m_ShowOptions = false;
	}

	void UIDropDown::Update(Vec2 mousePosition)
	{
		//If the button was pressed make all options visible.
		if (GetPressed())
		{
			if (GetMouseIsOver())
			{
				for (unsigned int i = 0; i < m_Options.size(); i++)
				{
					m_Options[i]->SetVisible(true);
				}
			}

			SetPressed(false);
		}

		Vec2 topLeft = Vec2(m_Position.x, m_Position.y + m_Size.y);
		Vec2 bottomRight = Vec2(m_Position.x + m_Size.x, m_Position.y);

		//Highlight the DropDown if the mouse is over it.
		if (IsInBox(mousePosition, topLeft, bottomRight))
		{
			SetMouseIsOver(true);
			m_Color = m_BGHighlightColor;
		}

		//If the mouse isn't over the DropDown or one of it's options, hide all drop down options.
		else
		{
			SetMouseIsOver(false);
			m_Color = m_BGNormalColor;

			for (unsigned int i = 0; i < m_Options.size(); i++)
			{
				m_Options[i]->SetVisible(false);
			}
		}
	}

	void UIDropDown::Draw(Renderer* renderer)
	{
		renderer->DrawRectangle(m_Position, m_Size, m_Color, m_Depth);

		renderer->DrawText(m_Font, m_Text, m_Position, ColorRGBA8(255, 255, 255, 255), TextAlignment::LEFT, m_Scale , m_Depth - 0.01f);
	}

	void UIDropDown::AddOption(const std::string& optionName)
	{
		//Create a button object and set it's text.
		UIButton* button = m_UISystem->AddButton(Vec2(0.0f, 0.0f));

		m_Options.push_back(button);
		m_Options.back()->SetText(optionName);
	}
}