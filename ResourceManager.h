#pragma once
#include <SDL/SDL_mixer.h>
#include <string>
#include <map>
#include "SpriteFont.h"
#include "Texture.h"
#include "Shader.h"
#include "ImageLoader.h"
#include "MessageBus.h"

namespace Deft
{
	class SpriteFont;

	//All game resources are stored in caches here.
	static class ResourceManager
	{
	public:
		ResourceManager() {};
		~ResourceManager();

		static void Initialize(MessageBus* MessageBus);

		static void Destroy();

		//Resource loading functions.
		static void AddTexture(const std::string& textureName, GLuint textureID, unsigned int width, unsigned int height);
		static void LoadTexture(const std::string& textureName, const std::string& filePath);
		static void LoadFont(const std::string& fontName, const std::string& filePath, int size);
		static void LoadShader(const std::string& shaderName);
		static void LoadSound(const std::string& soundName, const std::string& filePath);
		static void LoadMusic(const std::string& musicName, const std::string& filePath);
		static void MakeMaterial(const std::string& materialName, const std::string& textureName, const std::string& shaderName);

		//Debugging.
		static void OutputResourceList();

		//Setters.
		static void SetDefaultShader(const std::string& shaderName);
		static void SetDefaultFont(const std::string& fontName);

		//Getters.
		static Texture GetTexture(const std::string& textureName);
		static Material GetMaterial(const std::string& materialName);
		static SpriteFont* GetFont(const std::string& fontName);
		static Shader* GetShader(const std::string& shaderName);
		static Shader* GetShader(unsigned int shaderID);
		static Mix_Chunk* GetSound(const std::string& soundName);
		static Mix_Music* GetMusic(const std::string& musicName);
		static unsigned int GetResourceMemory() { return m_ResourceMemory; }
		static Shader* GetDefaultShader();
		static unsigned char GetDefaultShaderID();
		static SpriteFont* GetDefaultFont();
		static std::string GetDefaultFontName() { return m_DefaultFontName; }

	private:
		static bool m_IsInitialized;

		static MessageBus* m_MessageBus;			//Reference to the message bus for debugging messages.

		//Asset Caches.
		static std::map<std::string, Texture> m_TextureCache;			//Cache for 2D textures.
		static std::map<unsigned int, Shader*> m_ShaderCache;			//Cache for shader programs.
		static std::map<std::string, Material> m_MaterialCache;			//Cache for materials bound with specific shaders.
		static std::map<std::string, SpriteFont*> m_FontCache;			//Cache for fonts.
		static std::map<std::string, Mix_Chunk*> m_SoundCache;			//Cache for sound effects.
		static std::map<std::string, Mix_Music*> m_MusicCache;			//Cache for music tracks.

		//Keep track of how much memory is loaded into the game.
		static unsigned int m_ResourceMemory;

		//Default rendering assets.
		static std::string m_DefaultShaderName;
		static std::string m_DefaultFontName;
	};
}