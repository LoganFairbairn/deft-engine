#include "UIText.h"

namespace Deft 
{
	UIText::UIText()
	{
		m_FontName = ResourceManager::GetDefaultFontName();
		SetText("Blank");
	}

	void UIText::Update(Vec2 mousePosition)
	{
		Vec2 topLeft = Vec2(m_Position.x, m_Position.y + m_Size.y);
		Vec2 bottomRight = Vec2(m_Position.x + m_Size.x, m_Position.y);

		//Highlight the text button if the mouse is over it.
		if (IsInBox(mousePosition, topLeft, bottomRight))
		{
			SetMouseIsOver(true);
		}

		else
		{
			SetMouseIsOver(false);
		}
	}

	//Note: This uses sprite batching.
	//Draws the text to the screen.
	void UIText::Draw(Renderer* renderer)
	{
		SpriteFont* font = ResourceManager::GetFont(m_FontName);
		renderer->DrawText(font, m_Text, m_Position, m_Color, TextAlignment::LEFT, m_Scale, m_Depth);
	}

	//Sets the text and updates the bounding box.
	void UIText::SetText(std::string text)
	{
		m_Text = text;
		UpdateSize();
	}

	//Set the font for the text and update the bounding box.
	void UIText::SetFont(std::string fontName)
	{
		m_FontName = fontName;
		UpdateSize();
	}

	//Updates the bounding box and the size for the button.
	void UIText::UpdateSize()
	{
		SpriteFont* font = ResourceManager::GetFont(m_FontName);

		m_Size.x = font->MeasureText(m_Text.c_str()).x;
		m_Size.y = font->MeasureText(m_Text.c_str()).y;
	}
}