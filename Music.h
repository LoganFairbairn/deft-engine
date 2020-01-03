#pragma once
#include <SDL/SDL_mixer.h>
#include <string>

namespace Deft
{
	class Music
	{
	public:
		Music() {};
		~Music() {};

		void Play(int loops = 0);
		static void Pause();
		static void Stop();
		static void Resume();

		void SetSound(Mix_Music* music) { m_Music = music; }
		Mix_Music* GetMusic() { return m_Music; }

	private:
		Mix_Music* m_Music = nullptr;
	};
}