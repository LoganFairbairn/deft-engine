#pragma once
#include <string>
#include "UIObject.h"
#include "MathFunctions.h"

namespace Deft
{
	//This is a UIObject that can be added to the UI to display text.
	class UIText : public UIObject
	{
	public:
		UIText();
		~UIText() {};

		void Update(Vec2 mousePosition);
		void Draw(Renderer* renderer);

		//Setters.
		void SetText(std::string text);
		void SetFont(std::string fontName);
		void SetAlignment(TextAlignment textAlignment) { m_TextAlignment = textAlignment; }

		//Getters.
		std::string GetText() { return m_Text; }
		std::string GetFont() { return m_FontName; }
		TextAlignment GetTextAlignment() { return m_TextAlignment; }

	private:
		void UpdateSize();

		std::string m_Text;
		std::string m_FontName;		//Name of the font stored in the resource cache.
		TextAlignment m_TextAlignment;	
	};
}