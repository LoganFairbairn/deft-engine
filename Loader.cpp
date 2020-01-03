#include "Loader.h"

namespace Deft
{
	Loader::Loader()
	{
		m_LoadStep = 0;
	}

	Loader::~Loader()
	{
		//Clean up here if required.
	}

	//Draws the loading screen.
	void Loader::DrawLoading(Renderer* renderer)
	{
		//Draw loading bar.
		float barHeight = 45.0f;
		float barWidth = 400.0f;
		float topLeftX = (renderer->GetCameraWidth() * 0.5f) - (barWidth * 0.5f);
		float topLeftY = (renderer->GetCameraHeight() * 0.5f) - (barHeight * 0.5f);
		//renderer->DrawPercentBar(Vec2(topLeftX, topLeftY), Vec2(topLeftX + barWidth, topLeftY + barHeight), ColorRGBA8(255, 0, 0, 255), m_PercentCompleted);

		//Draw completion percent text.
		float x = renderer->GetCameraWidth() * 0.5f;
		float y = renderer->GetCameraHeight() * 0.5f;

		//Get the font for the loading screen.
		SpriteFont* font = ResourceManager::GetFont("Font_Loading");

		float halfTextHeight = font->GetFontHeight() * 0.5f;

		std::string text = "Loading - " + std::to_string((int)m_PercentCompleted) + "%";
		renderer->DrawText(font,
			text.c_str(),
			Vec2(x, y - (halfTextHeight)),
			ColorRGBA8(255, 255, 255, 255),
			TextAlignment::MIDDLE,
			Vec2(1.0f, 1.0f),
			0.0f);
	}

	//Unloads uneccessary assets, then loads assets in managable chunks.
	void Loader::Load()
	{
		switch (m_LoadStep)
		{
			case (unsigned int)LoadStep::LOADING_START:
			{
				//Unload any assets that aren't needed anymore.
			}
			break;

			case(unsigned int)LoadStep::LOADING_FONTS:
			{
				LoadFonts();
			}
			break;

			case(unsigned int)LoadStep::LOADING_SHADERS:
			{
				LoadShaders();
			}
			break;

			case(unsigned int)LoadStep::LOADING_TEXTURES:
			{
				LoadTextures();
			}
			break;

			case(unsigned int)LoadStep::LOADING_SOUNDS:
			{
				LoadSounds();
			}
			break;

			case(unsigned int)LoadStep::LOADING_MUSIC:
			{
				LoadMusic();
			}
			break;

			case (unsigned int)LoadStep::LOADING_FINISHED:
			{
				//
			}
			break;
		}

		//Calculate a loading completion.
		m_PercentCompleted = ((m_LoadStep / (float)LoadStep::LOADING_FINISHED)) * 100.0f;

		//If the loading isn't finished, move to the next load step.
		if (m_PercentCompleted != 100.0f)
		{
			m_LoadStep++;
		}
	}

	//Adds a texture to a que to be loader later.
	void Loader::AddTexture(const std::string& textureName, const std::string& filePath)
	{
		m_TextureQue.push_back(std::make_pair(textureName, filePath));
	}

	void Loader::AddShader(const std::string& shaderName, const std::string& filePath)
	{
		m_ShaderQue.push_back(std::make_pair(shaderName, filePath));
	}

	void Loader::AddFont(const std::string& fontName, const std::string& filePath, int size)
	{
		m_FontQue.push_back(std::make_pair(fontName, filePath));
		m_FontSizeQue.push_back(size);
	}

	void Loader::AddSound(const std::string& soundName, const std::string& filePath)
	{
		m_SoundQue.push_back(std::make_pair(soundName, filePath));
	}

	void Loader::AddMusic(const std::string& musicName, const std::string& filePath)
	{
		m_MusicQue.push_back(std::make_pair(musicName, filePath));
	}

	//Loads all of the textures in the texture que.
	void Loader::LoadTextures()
	{
		for (unsigned int i = 0; i < m_TextureQue.size(); i++)
		{
			//Load the texture.
			std::string textureName = m_TextureQue[i].first;
			std::string filePath = m_TextureQue[i].second;
			ResourceManager::LoadTexture(textureName, filePath);
		}

		//Remove all elements from the texture que.
		m_TextureQue.clear();
	}

	//Loads all of the shaders in the shader que.
	void Loader::LoadShaders()
	{
		for (unsigned int i = 0; i < m_ShaderQue.size(); i++)
		{
			//Load the shader.
			std::string shaderName = m_ShaderQue[i].first;
			ResourceManager::LoadShader(shaderName);
		}

		//Remove all elements from the que.
		m_ShaderQue.clear();
	}

	//Load all of the fonts in the font que.
	void Loader::LoadFonts()
	{
		for (unsigned int i = 0; i < m_FontQue.size(); i++)
		{
			//Load the texture.
			std::string fontName = m_FontQue[i].first;
			std::string filePath = m_FontQue[i].second;
			ResourceManager::LoadFont(fontName, filePath, m_FontSizeQue[i]);
		}

		//Remove all elements from the que.
		m_FontQue.clear();
		m_FontSizeQue.clear();
	}

	//Load all of the sounds in the sound que.
	void Loader::LoadSounds()
	{
		for (unsigned int i = 0; i < m_SoundQue.size(); i++)
		{
			//Load the texture.
			std::string soundName = m_SoundQue[i].first;
			std::string filePath = m_SoundQue[i].second;
			ResourceManager::LoadSound(soundName, filePath);
		}

		//Remove all elements from the que.
		m_SoundQue.clear();
	}

	//Load all of the music in the music que.
	void Loader::LoadMusic()
	{
		for (unsigned int i = 0; i < m_MusicQue.size(); i++)
		{
			//Load the texture.
			std::string musicName = m_MusicQue[i].first;
			std::string filePath = m_MusicQue[i].second;
			ResourceManager::LoadMusic(musicName, filePath);
		}

		//Remove all elements from the que.
		m_MusicQue.clear();
	}
}