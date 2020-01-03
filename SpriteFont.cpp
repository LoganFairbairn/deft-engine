#include "SpriteFont.h"

#include <SDL/SDL.h>
#include <iostream>		//Testing remove later.


int ClosestPow2(int i) 
{
	i--;
	int pi = 1;
	while (i > 0) 
	{
		i >>= 1;
		pi <<= 1;
	}
	return pi;
}

namespace Deft 
{
	SpriteFont::SpriteFont(const std::string& name, const char* file, int size)
	{
		m_Name = name;

		InitializeFont(file, size, FIRST_PRINTABLE_CHAR, LAST_PRINTABLE_CHAR);
	}

	SpriteFont::~SpriteFont()
	{
		//Clean up.
		Destroy();
	}

	void SpriteFont::InitializeFont(const char* filePath, int size)
	{
		InitializeFont(filePath, size, FIRST_PRINTABLE_CHAR, LAST_PRINTABLE_CHAR);
	}

	//Initializes a font.
	void SpriteFont::InitializeFont(const char* filePath, int fontSize, char firstChar, char lastChar)
	{
		//Initialize SDL_ttf if it's not already initialized.
		if (!TTF_WasInit()) 
		{
			TTF_Init();
		}

		//Open a .ttf font and check for errors.
		TTF_Font* font = TTF_OpenFont(filePath, fontSize);
		if (font == nullptr)
		{
			FatalError("Failed to open TTF font.");
		}

		//Store the font height and available font characters.
		m_FontHeight = TTF_FontHeight(font);
		m_CharStart = firstChar;
		m_CharLength = lastChar - firstChar + 1;

		//Set some padding for text.
		int padding = fontSize / 8;

		//Measure all the regions.
		glm::ivec4* glyphRects = new glm::ivec4[m_CharLength];

		int i = 0, advance;
		for (char c = firstChar; c <= lastChar; c++) 
		{
			TTF_GlyphMetrics(font, c, &glyphRects[i].x, &glyphRects[i].z, &glyphRects[i].y, &glyphRects[i].w, &advance);
			glyphRects[i].z -= glyphRects[i].x;
			glyphRects[i].x = 0;
			glyphRects[i].w -= glyphRects[i].y;
			glyphRects[i].y = 0;
			i++;
		}

		//Find best partitioning of glyphs in the texture.
		int rows = 1, width, height, bestWidth = 0, bestHeight = 0, area = MAX_TEXTURE_RES * MAX_TEXTURE_RES, bestRows = 0;
		std::vector<int>* bestPartition = nullptr;

		//Make rows for all of the characters in the font.
		while (rows <= m_CharLength)
		{
			height = rows * (padding + m_FontHeight) + padding;
			auto generatedRows = CreateRows(glyphRects, m_CharLength, rows, padding, width);

			//Make the width and the height of the texture a power of 2.
			width = ClosestPow2(width);
			height = ClosestPow2(height);

			// A texture must be feasible
			if (width > MAX_TEXTURE_RES || height > MAX_TEXTURE_RES) 
			{
				rows++;
				delete[] generatedRows;
				continue;
			}

			// Check for minimal area
			if (area >= width * height) 
			{
				if (bestPartition)
				{
					delete[] bestPartition;
				}

				bestPartition = generatedRows;
				bestWidth = width;
				bestHeight = height;
				bestRows = rows;
				area = bestWidth * bestHeight;
				rows++;
			}

			else 
			{
				delete[] generatedRows;
				break;
			}
		}

		// Can a bitmap font be made?
		if (!bestPartition) 
		{
			FatalError("Failed to map TTF font to texture. Try lowering resolution.");
		}

		//Create a GL texture.
		glGenTextures(1, &m_Material.texture.id);
		glBindTexture(GL_TEXTURE_2D, m_Material.texture.id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bestWidth, bestHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		//Cache the texture in the resource manager (this makes a drawable material for it).
		ResourceManager::AddTexture("FontTexture_" + m_Name, m_Material.texture.id, 0, 0);

		//Store the material here.
		m_Material = ResourceManager::GetMaterial("FontTexture_" + m_Name);

		//Now draw all the character glyphs.
		SDL_Color fg = { 255, 255, 255, 255 };
		int ly = padding;

		for (int ri = 0; ri < bestRows; ri++) 
		{
			int lx = padding;
			for (size_t ci = 0; ci < bestPartition[ri].size(); ci++) 
			{
				int gi = bestPartition[ri][ci];

				SDL_Surface* glyphSurface = TTF_RenderGlyph_Blended(font, (char)(firstChar + gi), fg);

				// Pre-multiplication occurs here
				unsigned char* sp = (unsigned char*)glyphSurface->pixels;
				int cp = glyphSurface->w * glyphSurface->h * 4;
				for (int i = 0; i < cp; i += 4) 
				{
					float a = sp[i + 3] / 255.0f;
					sp[i] = (unsigned char)((float)sp[i] * a);
					sp[i + 1] = sp[i];
					sp[i + 2] = sp[i];
				}

				// Save glyph image and update coordinates
				glTexSubImage2D(GL_TEXTURE_2D, 0, lx, bestHeight - ly - 1 - glyphSurface->h, glyphSurface->w, glyphSurface->h, GL_BGRA, GL_UNSIGNED_BYTE, glyphSurface->pixels);
				glyphRects[gi].x = lx;
				glyphRects[gi].y = ly;
				glyphRects[gi].z = glyphSurface->w;
				glyphRects[gi].w = glyphSurface->h;

				SDL_FreeSurface(glyphSurface);
				glyphSurface = nullptr;

				lx += glyphRects[gi].z + padding;
			}

			ly += m_FontHeight + padding;
		}

		//Draw the unsupported glyph.
		int rs = padding - 1;
		int* pureWhiteSquare = new int[rs * rs];
		memset(pureWhiteSquare, 0xffffffff, rs * rs * sizeof(int));
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rs, rs, GL_RGBA, GL_UNSIGNED_BYTE, pureWhiteSquare);
		delete[] pureWhiteSquare;
		pureWhiteSquare = nullptr;

		//Set some texture parameters.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		//Create character glyphs.
		m_Glyphs = new Glyph[m_CharLength + 1];
		for (i = 0; i < m_CharLength; i++) 
		{
			m_Glyphs[i].character = (char)(firstChar + i);
			//std::cout << m_Glyphs[i].character << std::endl;
			m_Glyphs[i].size = Vec2(glyphRects[i].z, glyphRects[i].w);
			m_Glyphs[i].uvRect = Vec4((float)glyphRects[i].x / (float)bestWidth,
									  (float)glyphRects[i].y / (float)bestHeight,
									  (float)glyphRects[i].z / (float)bestWidth,
									  (float)glyphRects[i].w / (float)bestHeight);
		}

		m_Glyphs[m_CharLength].character = ' ';
		m_Glyphs[m_CharLength].size = m_Glyphs[0].size;
		m_Glyphs[m_CharLength].uvRect = Vec4(0.0f, 0.0f, (float)rs / (float)bestWidth, (float)rs / (float)bestHeight);

		//Unbind the texture.
		glBindTexture(GL_TEXTURE_2D, 0);

		//Delete the glyph rectangles and the best partitions they aren't needed anymore.
		delete[] glyphRects;
		delete[] bestPartition;

		//We're finished with the font, close it.
		TTF_CloseFont(font);
	}

	//Cleans up memory when done with the selected font.
	void SpriteFont::Destroy() 
	{
		if (m_Material.texture.id != 0)
		{
			glDeleteTextures(1, &m_Material.texture.id);
			m_Material.texture.id = 0;
		}

		if (m_Glyphs) 
		{
			delete[] m_Glyphs;
			m_Glyphs = nullptr;
		}
	}

	std::vector<int>* SpriteFont::CreateRows(glm::ivec4* rects, int rectsLength, int r, int padding, int& w) 
	{
		// Blank initialize
		std::vector<int>* l = new std::vector<int>[r]();
		int* cw = new int[r]();
		for (int i = 0; i < r; i++) 
		{
			cw[i] = padding;
		}

		// Loop through all glyphs
		for (int i = 0; i < rectsLength; i++) 
		{
			// Find row for placement
			int ri = 0;
			for (int rii = 1; rii < r; rii++)
			{
				if (cw[rii] < cw[ri])
				{
					ri = rii;
				}
			}

			// Add width to that row
			cw[ri] += rects[i].z + padding;

			// Add glyph to the row list
			l[ri].push_back(i);
		}

		// Find the max width
		w = 0;
		for (int i = 0; i < r; i++) 
		{
			if (cw[i] > w) w = cw[i];
		}

		return l;
	}

	//Measures and returns the width and height of a chunk of text relative to the font.
	glm::vec2 SpriteFont::MeasureText(const char* s)
	{
		glm::vec2 size(0, m_FontHeight);
		float cw = 0;
		for (int si = 0; s[si] != 0; si++) 
		{
			char c = s[si];
			if (s[si] == '\n') 
			{
				size.y += m_FontHeight;
				if (size.x < cw)
				{
					size.x = cw;
				}
				cw = 0;
			}

			else 
			{
				// Check for correct glyph
				int gi = c - m_CharStart;
				if (gi < 0 || gi >= m_CharLength)
				{
					gi = m_CharLength;
				}
				cw += m_Glyphs[gi].size.x;
			}
		}

		if (size.x < cw)
		{
			size.x = cw;
		}
			
		return size;
	}

	//Draws text to the screen using sprite batching.
	//Note you must have already began sprite batching.
	//After you finish, end sprite batching.
	void SpriteFont::DrawText(Renderer* renderer, const char* text, Vec2 position, Vec2 scale, float depth, ColorRGBA8 color, TextAlignment alignment)
	{
		if (renderer == nullptr)
		{
			FatalError("Text drawing error: Renderer is nullptr.");
		}
		
		Vec2 textPosition = position;

		//Apply text alignment before drawing.
		if (alignment == TextAlignment::MIDDLE)
		{
			textPosition.x -= MeasureText(text).x * scale.x / 2;
		}

		else if (alignment == TextAlignment::RIGHT)
		{
			textPosition.x -= MeasureText(text).x * scale.x;
		}

		//Adjust the text so it start drawing from the top.
		textPosition.y += MeasureText(text).y - m_FontHeight * scale.y;

		//
		for (int stringIndex = 0; text[stringIndex] != 0; stringIndex++)
		{
			char c = text[stringIndex];
			if (text[stringIndex] == '\n')
			{
				textPosition.x = position.x;
				textPosition.y -= m_FontHeight * scale.y;
			}

			else
			{
				// Check for correct glyph
				int index = c - m_CharStart;
				if (index < 0 || index >= m_CharLength)
				{
					index = m_CharLength;
				}

				//Apply the given scale.
				float sizeX = m_Glyphs[index].size.x * scale.x;
				float sizeY = m_Glyphs[index].size.y * scale.y;

				//Material material, Vec2 position, float rotation, Vec2 pivot, ColorRGBA8 color, float depth, Vec4 uvRectangle)
				renderer->DrawSprite(m_Material,
					textPosition,
					Vec2(sizeX, sizeY),
					0.0f,
					Vec2(0.0f, 0.0f),
					color,
					depth,
					m_Glyphs[index].uvRect);

				//Add to the current text position before drawing the next character.
				textPosition.x += m_Glyphs[index].size.x * scale.x;
			}
		}
	}
}