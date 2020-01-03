#include "UIDropDownMenu.h"

namespace Deft
{
	UIDropDownMenu::UIDropDownMenu(UISystem* system, Vec2 position)
	{
		m_UISystem = system;
		m_Position = position;

		//Default values.
		m_BackgroundColor = ColorRGBA8(85, 85, 85, 255);
		m_Size = Vec2(0.0f, 0.0f);
		m_Depth = -0.9f;
	}

	void UIDropDownMenu::Draw(Renderer* renderer)
	{
		renderer->DrawRectangle(m_Position, m_Size, m_BackgroundColor, m_Depth);
	}

	//Adds a new category button.
	void UIDropDownMenu::AddCategory(const std::string& categoryName) 
	{
		UIDropDown* dropDown = m_UISystem->AddDropDown(Vec2(0.0f, 0.0f), categoryName);
		float padding = dropDown->GetBGPadding();

		dropDown->SetColor(m_BackgroundColor);
		dropDown->SetGroup(UIGroup::UI_GROUP_EDITOR);
		dropDown->SetDepth(m_Depth - 0.01f);

		//Move the DropDown based on how many categories there are.
		float x = 0.0f;
		for (unsigned int i = 0; i < m_DropDowns.size(); i++)
		{
			x += m_DropDowns[i]->GetSize().x;
		}

		dropDown->SetPosition(Vec2(m_Position.x + x, m_Position.y));

		//Add the DropDown to the list.
		m_DropDowns.push_back(dropDown);

		//Adjust the size of the menu.
		m_Size.y += dropDown->GetSize().y;
	}

	//Creates a UIButton and adds it as an option under the given category.
	void UIDropDownMenu::AddOption(const std::string& categoryName, const std::string& optionName)
	{
		for (unsigned int i = 0; i < m_DropDowns.size(); i++)
		{
			if (m_DropDowns[i]->GetText() == categoryName)
			{
				m_DropDowns[i]->AddOption(optionName);

				break;
			}
		}
	}

	//Sets the position of the UIDropDownMenu and moves all UIButtons attatched to it.
	void UIDropDownMenu::SetPosition(Vec2 position)
	{
		m_Position = position;

		float x = m_Position.x;
		for (unsigned int i = 0; i < m_DropDowns.size(); i++)
		{
			m_DropDowns[i]->SetPosition(Vec2(x, m_Position.y));
			x += m_DropDowns[i]->GetSize().x;
		}
	}
}