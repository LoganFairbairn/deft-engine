#pragma once
#include "UIObject.h"

namespace Deft
{
	class UIToggle : public UIObject
	{
	public:
		UIToggle() {};
		~UIToggle() {};

		void Update(Vec2 mousePosition);
		void Draw(Renderer* renderer);

		void Toggle() { m_Toggle = !m_Toggle; }

		//Setters.
		void SetToggle(bool toggle) { m_Toggle = toggle; }
		void SetNormalColor(ColorRGBA8 normalColor) { m_NormalColor = normalColor; }
		void SetHighlightColor(ColorRGBA8 highlightedColor) { m_HighlightColor = highlightedColor; }
		void SetPressedColor(ColorRGBA8 pressedColor) { m_PressedColor = pressedColor; }
		void SetDisabledColor(ColorRGBA8 disabledColor) { m_DisabledColor = disabledColor; }

		//Getters.
		bool GetToggle() { return m_Toggle; };
		ColorRGBA8 GetNormalColor() { return m_NormalColor; }
		ColorRGBA8 GetHighlightColor() { return m_HighlightColor; }
		ColorRGBA8 GetPressedColor() { return m_PressedColor; }
		ColorRGBA8 GetDisabledColor() { return m_DisabledColor; }

	private:
		bool m_Toggle;

		Material m_Graphic;
		Material m_Checkmark;

		ColorRGBA8 m_NormalColor;
		ColorRGBA8 m_HighlightColor;
		ColorRGBA8 m_PressedColor;
		ColorRGBA8 m_DisabledColor;
	};
}