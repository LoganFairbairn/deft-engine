#include "ResourceManager.h"

namespace Deft
{
	MessageBus* ResourceManager::m_MessageBus;
	bool ResourceManager::m_IsInitialized = false;
	unsigned int ResourceManager::m_ResourceMemory = 0;
	std::map<std::string, Texture> ResourceManager::m_TextureCache;
	std::map<std::string, Material> ResourceManager::m_MaterialCache;
	std::map<unsigned int, Shader*> ResourceManager::m_ShaderCache;
	std::map<std::string, SpriteFont*> ResourceManager::m_FontCache;
	std::map<std::string, Mix_Chunk*> ResourceManager::m_SoundCache;
	std::map<std::string, Mix_Music*> ResourceManager::m_MusicCache;
	std::string ResourceManager::m_DefaultShaderName;
	std::string ResourceManager::m_DefaultFontName;

	ResourceManager::~ResourceManager()
	{
		Destroy();
	}

	void ResourceManager::Initialize(MessageBus* MessageBus)
	{
		m_MessageBus = MessageBus;

		//Make sure the audio engine isn't already initialized.
		if (m_IsInitialized)
		{
			FatalError("AudioEngine is already initialized.");
		}

		//Bitwise operator can be used to support multiple audio file types.
		if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1)
		{
			FatalError("Mix_Init error: " + std::string(Mix_GetError()));
		}

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
		{
			FatalError("Mix_Init error: " + std::string(Mix_GetError()));
		}

		//This object has been initialized without errors.
		m_IsInitialized = true;
	}

	//Cleans up all memory used by this object.
	void ResourceManager::Destroy()
	{
		if (m_IsInitialized)
		{
			m_IsInitialized = false;

			//Unload the sound effect chunks.
			for (auto& i : m_SoundCache)
			{
				Mix_FreeChunk(i.second);
			}

			//Unload the music chunks.
			for (auto& i : m_MusicCache)
			{
				Mix_FreeMusic(i.second);
			}

			//Clear the maps used to hold the audio files.
			m_SoundCache.clear();
			m_MusicCache.clear();

			//Closes the audio device, this stops SDL_mixer functions to not work.
			Mix_CloseAudio();

			//Cleans up all dynamically loaded library handles, freeing memory.
			Mix_Quit();

			//Delete all textures that exist within the texturte cache.
			for (auto i = m_TextureCache.begin(); i != m_TextureCache.end(); i++)
			{
				if (glIsTexture(i->second.id) == GL_TRUE)
				{
					glDeleteTextures(1, &i->second.id);
				}
			}

			//Clear all of the maps used for the caches.
			m_TextureCache.clear();
			m_ShaderCache.clear();
			m_MaterialCache.clear();
			m_FontCache.clear();
		}
	}

	//Adds a texture that is already created to the resource cache.
	void ResourceManager::AddTexture(const std::string& textureName, GLuint textureID, unsigned int width, unsigned int height)
	{
		//Search for the texture inside the cache.
		std::map<std::string, Texture>::iterator i = m_TextureCache.find(textureName);

		//If the texture is not found in the texture map already, load the texture and then store it in the cache.
		if (i == m_TextureCache.end())
		{
			//Load the new texture and store it in the cache.
			Texture newTexture;
			newTexture.id = textureID;
			newTexture.width = width;
			newTexture.height = height;
			m_TextureCache.insert(make_pair(textureName, newTexture));
		}

		//Make a material with the default shader for this texture.
		MakeMaterial(textureName, textureName, GetDefaultShader()->GetName());

		//TODO: Add to memory.
	}

	//Loads a texture if it doesn't already exist in the cache.
	void ResourceManager::LoadTexture(const std::string& textureName, const std::string& filePath)
	{
		//Search for the texture inside the cache.
		std::map<std::string, Texture>::iterator i = m_TextureCache.find(textureName);

		//If the texture is not found in the texture map already, load the texture and then store it in the cache.
		if (i == m_TextureCache.end())
		{
			//Load the new texture and store it in the cache.
			Texture newTexture = ImageLoader::LoadPNG(filePath);
			m_TextureCache.insert(make_pair(textureName, newTexture));
		}

		//Make a material with the default shader for this texture.
		MakeMaterial(textureName, textureName, GetDefaultShader()->GetName());

		//Add to memory.
		m_ResourceMemory += sizeof(Texture);
	}

	//Loads a font if it doens't already exist in the cache.
	void ResourceManager::LoadFont(const std::string& fontName, const std::string& filePath, int size)
	{
		//Search for the texture inside the cache.
		std::map<std::string, SpriteFont*>::iterator i = m_FontCache.find(fontName);

		//If the font is not found in the texture map already, load the texture and then store it in the cache.
		if (i == m_FontCache.end())
		{
			//Load the new font and store it in the cache.
			SpriteFont* spriteFont = new SpriteFont(fontName, filePath.c_str(), size);
			m_FontCache.insert(make_pair(fontName, spriteFont));
		}

		//Add to memory.
		m_ResourceMemory += sizeof(SpriteFont);
	}

	//Loads a shader if it doens't already exist in the cache.
	void ResourceManager::LoadShader(const std::string& shaderName)
	{
		//Create a new shader.
		Shader* shader = new Shader();

		//Give the shader an ID number and set it's name.
		shader->SetID((unsigned int)m_ShaderCache.size());
		shader->SetName(shaderName);

		//Insert the shader into the cache.
		m_ShaderCache.insert(std::make_pair(shader->GetID(), shader));

		//Add to memory.
		m_ResourceMemory += sizeof(Shader);
	}

	//Loads a sound if it doens't already exist in the cache.
	void ResourceManager::LoadSound(const std::string& soundName, const std::string& filePath)
	{
		//Try to find the audio in the cache.
		auto i = m_SoundCache.find(soundName);

		//If there sound effect isn't cached already, load it.
		if (i == m_SoundCache.end())
		{
			Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());

			//Check for any errors that occured while loading.
			if (chunk == nullptr)
			{
				FatalError("Mix_LoadWAV error: " + std::string(Mix_GetError()) + "\n" + "File path: " + filePath);
			}

			//Cache the sound.
			m_SoundCache[soundName] = chunk;
		}

		//Add to memory.
		m_ResourceMemory += sizeof(Mix_Chunk);
	}

	//Loads music if it doesn't already exist in the cache.
	void ResourceManager::LoadMusic(const std::string& musicName, const std::string& filePath)
	{
		//Try to find the audio in the cache.
		auto i = m_MusicCache.find(musicName);

		//Music isn't in the cache, load it.
		if (i == m_MusicCache.end())
		{
			//If there sound effect isn't cached already, load it.
			Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());

			//Check for any errors that occured while loading.
			if (mixMusic == nullptr)
			{
				FatalError("Mix_LoadMUS error: " + std::string(Mix_GetError()) + "\n" + "File path: " + filePath);
			}

			//Cache the music.
			m_MusicCache[musicName] = mixMusic;
		}
	}

	//Only make materials when you need them.
	//Makes a material by binding a texture and a shader.
	void ResourceManager::MakeMaterial(const std::string& materialName, const std::string& textureName, const std::string& shaderName)
	{
		//Make sure there isn't already a material with the given name.
		auto i = m_MaterialCache.find(materialName);

		//If there isn't a material with the given name, create one and add it to the cache.
		if (i == m_MaterialCache.end())
		{
			Texture texture = GetTexture(textureName);
			Shader* shader = GetShader(shaderName);
			
			//Create a new material with the given values.
			Material material;
			material.shaderID = shader->GetID();
			material.texture = texture;

			//Set a material ID number.
			material.materialID = m_MaterialCache.size();

			//Insert the material into the cache.
			m_MaterialCache.insert(make_pair(materialName, material));
		}

		else
		{
			FatalError("Material already in cache: " + materialName);
		}
	}

	//Outputs a list of all resources.
	void ResourceManager::OutputResourceList()
	{
		m_MessageBus->LogMessage("\n");

		//Output data for all of the textures.
		m_MessageBus->LogMessage("Textures Cached: " + std::to_string(m_TextureCache.size()));
		unsigned int resourceNumber = 0;
		for(auto i = m_TextureCache.begin(); i != m_TextureCache.end(); i++)
		{
			std::string resourceInfo = "Texture - OpenGL Texture ID " + std::to_string(i->second.id) + " - " + i->first;
			m_MessageBus->LogMessage(resourceInfo);

			resourceNumber++;
		}

		m_MessageBus->LogMessage("\n");

		//Output data for all of the shaders.
		m_MessageBus->LogMessage("Shaders Cached: " + std::to_string(m_ShaderCache.size()));
		resourceNumber = 0;
		for (auto i = m_ShaderCache.begin(); i != m_ShaderCache.end(); i++)
		{
			std::string resourceInfo = "Shader - ID " + std::to_string(i->second->GetID()) + " - " + i->second->GetName();
			m_MessageBus->LogMessage(resourceInfo);

			resourceNumber++;
		}

		m_MessageBus->LogMessage("\n");

		//Output data for all of the materials.
		resourceNumber = 0;
		m_MessageBus->LogMessage("Materials Cached: " + std::to_string(m_MaterialCache.size()));
		for (auto i = m_MaterialCache.begin(); i != m_MaterialCache.end(); i++)
		{
			std::string resourceInfo = "Material - ID " + std::to_string(i->second.materialID) + " - " + i->first;
			m_MessageBus->LogMessage(resourceInfo);

			resourceNumber++;
		}

		m_MessageBus->LogMessage("\n");

		//Output data for all of the fonts.
		m_MessageBus->LogMessage("Fonts Cached: " + std::to_string(m_FontCache.size()));
		resourceNumber = 0;
		for (auto i = m_FontCache.begin(); i != m_FontCache.end(); i++)
		{
			std::string resourceInfo = "Font - " + i->first;
			m_MessageBus->LogMessage(resourceInfo);

			resourceNumber++;
		}

		m_MessageBus->LogMessage("\n");

		//Output data for all of the sound effects.
		m_MessageBus->LogMessage("Sound Effects Cached: " + std::to_string(m_SoundCache.size()));
		resourceNumber = 0;
		for (auto i = m_SoundCache.begin(); i != m_SoundCache.end(); i++)
		{
			std::string resourceInfo = "Sound - " + i->first;
			m_MessageBus->LogMessage(resourceInfo);

			resourceNumber++;
		}

		m_MessageBus->LogMessage("\n");

		//Output data for all of the music tracks.
		m_MessageBus->LogMessage("Music Tracks Cached: " + std::to_string(m_MusicCache.size()));
		resourceNumber = 0;
		for (auto i = m_MusicCache.begin(); i != m_MusicCache.end(); i++)
		{
			std::string resourceInfo = "Music - " + i->first;
			m_MessageBus->LogMessage(resourceInfo);

			resourceNumber++;
		}

		m_MessageBus->LogMessage("\n");
	}

	//Sets the deault shader for the game.
	void ResourceManager::SetDefaultShader(const std::string& shaderName)
	{
		m_DefaultShaderName = shaderName;
	}

	//Sets the default font for the game.
	void  ResourceManager::SetDefaultFont(const std::string& fontName)
	{
		m_DefaultFontName = fontName;
	}

	//Returns a texture if it's already inside the font cache.
	Texture ResourceManager::GetTexture(const std::string& textureName)
	{
		//Search for the texture inside the cache.
		std::map<std::string, Texture>::iterator i = m_TextureCache.find(textureName);

		//If the texture is not found in the texture map already, load the texture and then store it in the cache.
		if (i == m_TextureCache.end())
		{
			FatalError("Texture " + textureName + " not found in texture cache.");
		}

		else
		{
			return m_TextureCache.at(textureName);
		}
	}

	//Returns a material if it's already inside the cache.
	Material ResourceManager::GetMaterial(const std::string& materialName)
	{
		//Search for the material inside the cache.
		std::map<std::string, Material>::iterator i = m_MaterialCache.find(materialName);

		//If the texture is not found in the texture map already, load the texture and then store it in the cache.
		if (i == m_MaterialCache.end())
		{
			FatalError("Material " + materialName + " not found in material cache.");
		}

		else
		{
			return m_MaterialCache.at(materialName);
		}
	}

	//Returns a font if it's already inside the font cache.
	SpriteFont* ResourceManager::GetFont(const std::string& fontName)
	{
		//Search for the texture inside the cache.
		std::map<std::string, SpriteFont*>::iterator i = m_FontCache.find(fontName);

		//If the font is not found in the texture map already, load the texture and then store it in the cache.
		if (i == m_FontCache.end())
		{
			FatalError("Font " + fontName + " not found in texture cache.");
		}

		else
		{
			return m_FontCache.at(fontName);
		}
	}

	//Returns a shader with the given name if it's inside the cache.
	Shader* ResourceManager::GetShader(const std::string& shaderName)
	{
		for (unsigned int i = 0; i < m_ShaderCache.size(); i++)
		{
			if (m_ShaderCache.at(i)->GetName() == shaderName)
			{
				return m_ShaderCache.at(i);
			}
		}
	}

	//Returns a shader with the given ID if it's inside the cache.
	Shader* ResourceManager::GetShader(unsigned int shaderID)
	{
		//Search for the shader inside the cache.
		std::map<unsigned int, Shader*>::iterator i = m_ShaderCache.find(shaderID);

		//If the texture is not found in the cache already, load the shader and then store it in the cache.
		if (i == m_ShaderCache.end())
		{
			FatalError("Shader with ID " + std::to_string(shaderID) + " not found in shader cache.");
		}

		else
		{
			return m_ShaderCache.at(shaderID);
		}
	}

	//Returns a sound if it's already inside the sound cache.
	Mix_Chunk* ResourceManager::GetSound(const std::string& soundName)
	{
		//Try to find the audio in the cache.
		auto i = m_SoundCache.find(soundName);

		//If the sound isn't in the cache already, throw an error.
		if (i == m_SoundCache.end())
		{
			FatalError("Sound " + soundName + " not found in sound cache.");
		}

		else
		{
			return m_SoundCache.at(soundName);
		}
	}

	//Returns music if it's already inside the music cache.
	Mix_Music* ResourceManager::GetMusic(const std::string& musicName)
	{
		auto i = m_MusicCache.find(musicName);

		//Music isn't in the cache, throw an error.
		if (i == m_MusicCache.end())
		{
			FatalError("Music " + musicName + " not found in texture cache.");
		}

		else
		{
			return m_MusicCache.at(musicName);
		}
	}

	//Gets the default shader.
	Shader* ResourceManager::GetDefaultShader()
	{
		return GetShader(m_DefaultShaderName);
	}

	//Gets the ID of the default shader.
	unsigned char ResourceManager::GetDefaultShaderID()
	{
		return GetShader(m_DefaultShaderName)->GetID();
	}

	//Gets the set default font.
	SpriteFont* ResourceManager::GetDefaultFont()
	{
		return GetFont(m_DefaultFontName);
	}
}