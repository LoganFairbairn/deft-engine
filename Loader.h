#pragma once
#include "ResourceManager.h"

namespace Deft
{
	//These are the step for loading the game.
	enum class LoadStep
	{
		LOADING_START = 0,
		LOADING_FONTS,
		LOADING_SHADERS,
		LOADING_TEXTURES,
		LOADING_SOUNDS,
		LOADING_MUSIC,
		LOADING_FINISHED
	};

	//This is responsible for loading assets in chunks.
	//Add assets to a que, load it during a loading screen.
	class Loader
	{
	public:
		Loader();
		~Loader();

		void DrawLoading(Renderer* renderer);
		void Load();

		void AddTexture(const std::string& textureName, const std::string& filePath);
		void AddShader(const std::string& shaderName, const std::string& filePath);
		void AddFont(const std::string& fontName, const std::string& filePath, int size);
		void AddSound(const std::string& soundName, const std::string& filePath);
		void AddMusic(const std::string& musicName, const std::string& filePath);

		void LoadTextures();
		void LoadShaders();
		void LoadFonts();
		void LoadSounds();
		void LoadMusic();

		float GetPercentCompleted() { return m_PercentCompleted; }

	private:
		unsigned int m_LoadStep = 0;
		float m_PercentCompleted = 0;

		//Ques for each asset type.
		std::vector<std::pair<std::string, std::string>> m_TextureQue;
		std::vector<std::pair<std::string, std::string>> m_ShaderQue;
		std::vector<std::pair<std::string, std::string>> m_FontQue;
		std::vector<int> m_FontSizeQue;
		std::vector<std::pair<std::string, std::string>> m_SoundQue;
		std::vector<std::pair<std::string, std::string>> m_MusicQue;
	};
}