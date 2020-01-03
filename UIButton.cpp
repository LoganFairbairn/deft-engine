#include "UIButton.h"

namespace Deft
{
	UIButton::UIButton()
	{
		m_Description = "";

		m_Pressed = false;
		m_Disabled = false;
		m_HasGraphic = false;

		m_Font = ResourceManager::GetDefaultFont();
		m_Text = "";
		m_TextColor = ColorRGBA8(255, 255, 255, 255);
		m_TextAlignment = TextAlignment::LEFT;

		m_BGNormalColor = ColorRGBA8(255, 255, 255, 255);
		m_BGHighlightColor = ColorRGBA8(51, 150, 242, 255);
		m_BGDisabledColor = ColorRGBA8(150, 150, 150, 255);
		m_BGPadding = 2.0f;
	}

	void UIButton::Update(Vec2 mousePosition)
	{
		Vec2 topLeft = Vec2(m_Position.x, m_Position.y + m_Size.y);
		Vec2 bottomRight = Vec2(m_Position.x + m_Size.x, m_Position.y);

		//Highlight the button if the mouse is over it.
		if (IsInBox(mousePosition, topLeft, bottomRight))
		{
			SetMouseIsOver(true);
		}

		else
		{
			SetMouseIsOver(false);
		}
	}

	void UIButton::Draw(Renderer* renderer)
	{
		Vec2 textPosition = m_Position;

		//Draw the button's background if it has one.
		if (m_DrawBackground)
		{
			if (m_Disabled == false)
			{
				if (m_MouseIsOver)
				{
					m_Color = m_BGHighlightColor;
				}

				else
				{
					m_Color = m_BGNormalColor;
				}
			}

			//Apply custom background size.
			if (m_CustomBGSize.x != 0.0f)
			{
				renderer->DrawRectangle(m_Position, m_CustomBGSize, m_Color, m_Depth + 0.001f);
				textPosition.y = m_Position.y + (m_CustomBGSize.y * 0.5f) - (m_Font->GetFontHeight() * 0.5f);
			}

			else
			{
				renderer->DrawRectangle(m_Position, m_Size, m_Color, m_Depth + 0.001f);
				textPosition.y = m_Position.y + (m_Size.y * 0.5f) - (m_Font->GetFontHeight() * 0.5f);
			}

			//Apply background padding.
			textPosition.x += m_BGPadding;
		}

		//Draw the button's graphic if it has one.
		if (m_HasGraphic == true)
		{
			if (m_MouseIsOver)
			{
				renderer->DrawSprite(m_MouseOverGraphic, m_Position, m_Scale, m_Pivot, ColorRGBA8(255, 255, 255, 255), m_Depth);
			}
			
			else
			{
				renderer->DrawSprite(m_Graphic, m_Position, m_Scale, m_Pivot, ColorRGBA8(255, 255, 255, 255), m_Depth);
			}

			textPosition.y = m_Position.y + (m_Graphic.texture.height * 0.5f) - (m_Font->GetFontHeight() * 0.5f);
		}

		//Draw the button's text.
		if (m_Text != "")
		{
			//Apply text alignment (when using rectangle background or graphic)
			if (m_TextAlignment == TextAlignment::MIDDLE)
			{
				if (m_HasGraphic)
				{
					textPosition.x = m_Position.x + (m_Graphic.texture.width * 0.5f) - (m_Font->MeasureText(m_Text.c_str()).x * 0.5f);
				}

				else
				{
					textPosition.x = m_Position.x + (m_Size.x * 0.5f) - (m_Font->MeasureText(m_Text.c_str()).x * 0.5f);
				}
			}

			if (m_TextAlignment == TextAlignment::RIGHT)
			{
				if (m_HasGraphic)
				{
					textPosition.x = m_Position.x + m_Graphic.texture.width - m_Font->MeasureText(m_Text.c_str()).x * 0.5f;
				}

				else
				{
					textPosition.x = m_Position.x + m_Size.x - m_Font->MeasureText(m_Text.c_str()).x;
				}
			}

			renderer->DrawText(m_Font, m_Text, textPosition, m_TextColor, TextAlignment::LEFT, m_Scale, m_Depth - 0.01f);
		}
	}

	//Set graphics for the button and updates it's size.
	void UIButton::SetGraphics(Material graphic, Material mouseOverGraphic)
	{
		m_Graphic = graphic;
		m_MouseOverGraphic = mouseOverGraphic;

		m_HasGraphic = true;
		UpdateSize();
	}

	//Sets the text for the button and updates it's size.
	void UIButton::SetText(const std::string& text)
	{
		m_Text = text;
		UpdateSize();
	}

	//Sets the font for the button and updates it's size.
	void UIButton::SetFont(const std::string& font)
	{
		m_Font = ResourceManager::GetFont(font);
		UpdateSize();
	}

	//Updates the size of the button based on button settings.
	void UIButton::UpdateSize()
	{
		//Use button graphic for size.
		if (m_Graphic.texture.width != 0 && m_Graphic.texture.height != 0)
		{
			m_Size.x = m_Graphic.texture.width;
			m_Size.y = m_Graphic.texture.height;
		}

		//Use background for size if there is no graphic.
		else if (m_DrawBackground)
		{
			//If there is no custom size set for this button, use the background padding and the button text to calculate the size.
			if (m_CustomBGSize.x == 0.0f)
			{
				float width = m_Font->MeasureText(m_Text.c_str()).x + (m_BGPadding * 2);
				float height = m_Font->MeasureText(m_Text.c_str()).y + (m_BGPadding * 2);
				m_Size = Vec2(width, height);
			}

			else
			{
				m_Size = m_CustomBGSize;
			}
		}

		//Use text for size if there is no background.
		else
		{
			m_Size.x = m_Font->MeasureText(m_Text.c_str()).x;
			m_Size.y = m_Font->MeasureText(m_Text.c_str()).y;
		}
	}
}