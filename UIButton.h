#pragma once
#include "UIObject.h"
#include "MathFunctions.h"
#include "Shape.h"
#include "Rectangle.h"

namespace Deft
{
	//This is a UIObject that can be clicked to perform an action.
	class UIButton : public UIObject
	{
	public:
		UIButton();
		~UIButton() {};

		void Update(Vec2 mousePosition);
		void Draw(Renderer* renderer);

		//Setters.
		void SetDisabled(bool disabled) { m_Disabled = disabled; }
		void SetPressed(bool pressed) { m_Pressed = pressed; }

		void DrawBackground(bool drawBackground) { m_DrawBackground = drawBackground; }
		void SetCustomBGSize(Vec2 size) { m_CustomBGSize = size; }
		void SetBGNormalColor(ColorRGBA8 normalColor) { m_BGNormalColor = normalColor; }
		void SetBGHighlightColor(ColorRGBA8 highlightedColor) { m_BGHighlightColor = highlightedColor; }
		void SetBGDisabledColor(ColorRGBA8 disabledColor) { m_BGDisabledColor = disabledColor; }
		void SetBackgroundPadding(float backgroundPadding) { m_BGPadding = backgroundPadding; }

		void SetGraphics(Material graphic, Material mouseOverGraphic);

		void SetText(const std::string& text);
		void SetFont(const std::string& font);
		void SetTextColor(ColorRGBA8 color) { m_TextColor = color; }
		void SetTextAlignment(TextAlignment textAlignment) { m_TextAlignment = textAlignment; }

		//Getters.
		bool GetDisabled() { return m_Disabled; }
		bool GetPressed() { return m_Pressed; }

		ColorRGBA8 GetBGNormalColor() { return m_BGNormalColor; }
		ColorRGBA8 GetBGHighlightColor() { return m_BGHighlightColor; }
		ColorRGBA8 GetBGDisabledColor() { return m_BGDisabledColor; }
		float GetBGPadding() { return m_BGPadding; }

		bool GetHasGraphic() { return m_HasGraphic; }
		Material GetGraphic() { return m_Graphic; }
		Material GetMouseOverGraphic() { return m_MouseOverGraphic; }

		std::string GetText() { return m_Text; }
		SpriteFont* GetFont() { return m_Font; }
		float GetFontHeight() { return m_Font->GetFontHeight(); }
		ColorRGBA8 GetTextColor() { return m_TextColor; }
		TextAlignment GetTextAlignment() { return m_TextAlignment; }

	protected:
		void UpdateSize();

		bool m_Pressed = false;
		bool m_Disabled = false;

		//Background.
		bool m_DrawBackground = false;				//Set true if you want a rectangle background for the button.
		Vec2 m_CustomBGSize = Vec2(0.0f, 0.0f);
		float m_BGPadding = 2.0f;
		ColorRGBA8 m_BGNormalColor;
		ColorRGBA8 m_BGHighlightColor;
		ColorRGBA8 m_BGDisabledColor;

		//Graphic.
		bool m_HasGraphic = false;		//True if the button uses a graphic.
		Material m_Graphic;
		Material m_MouseOverGraphic;

		//Text.
		std::string m_Text;
		SpriteFont* m_Font;
		ColorRGBA8 m_TextColor;
		TextAlignment m_TextAlignment;
	};
}