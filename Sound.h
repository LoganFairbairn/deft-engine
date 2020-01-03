#pragma once
#include <SDL/SDL_mixer.h>
#include <string>

#include "DeftErrors.h"

namespace Deft
{
	class Sound
	{
	public:
		Sound() {};
		~Sound() {};

		void Play(int loops = 0);
		
		void SetSound(Mix_Chunk* chunk) { m_Chunk = chunk; }
		Mix_Chunk* GetSound() { return m_Chunk; }

	private:
		Mix_Chunk* m_Chunk = nullptr;
	};
}