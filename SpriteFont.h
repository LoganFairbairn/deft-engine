#pragma once
/*
This is a modified version of the SpriteFont class from the
Seed Of Andromeda source code.
Use it for any of your projects, commercial or otherwise,
free of charge, but do not remove this disclaimer.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS
ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
OF THIS SOFTWARE.

November 28 2014
Original Author: Cristian Zaloj
Modified By: Benjamin Arnold
Modified By: Logan Fairbairn (for Deft Engine).
*/

#pragma once

#ifndef SpriteFont_h__
#define SpriteFont_h__

#include <TTF/SDL_ttf.h>
#include <glm/glm.hpp>
#include <map>
#include <vector>

#include "FatalErrors.h"
#include "Vector.h"
#include "Vertex.h"
#include "Texture.h"
#include "Renderer.h"
#include "TextAlignment.h"
#include "ResourceManager.h"

namespace Deft 
{
	struct Glyph 
	{
	public:
		char character;
		Vec4 uvRect;
		Vec2 size;
	};

#define MAX_TEXTURE_RES 4096
#define FIRST_PRINTABLE_CHAR ((char)32)
#define LAST_PRINTABLE_CHAR ((char)126)

	class Renderer;

	//Create a new SpriteFont for every font in your game.
	//Use the SpriteFont by...
	//1. Initialize the font with set parameters.
	//2. Draw the desired text using DrawText.
	class SpriteFont 
	{
	public:
		SpriteFont(const std::string& name, const char* font, int size);
		~SpriteFont();

		void InitializeFont(const char* font, int size);
		void InitializeFont(const char* font, int size, char firstChar, char lastChar);
		void Destroy();

		void DrawText(Renderer* renderer, const char* s, Vec2 position, Vec2 scale, float depth, ColorRGBA8 color, TextAlignment alignment);

		glm::vec2 MeasureText(const char* s);
		int GetFontHeight() const { return m_FontHeight; }
		int GetCharacterStart() const { return m_CharStart; }
		int GetCharacterLength() const { return m_CharLength; }

	private:
		static std::vector<int>* CreateRows(glm::ivec4* rects, int rectsLength, int r, int padding, int& w);

		std::string m_Name;
		int m_CharStart;
		int m_CharLength;
		Glyph* m_Glyphs;
		int m_FontHeight;

		Material m_Material;
		GLuint m_TextureID;
	};
}
#endif // SpriteFont_h__