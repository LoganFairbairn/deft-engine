#include "Music.h"

namespace Deft
{
	//Plays the specified music track.
	void Music::Play(int loops)
	{
		Mix_PlayMusic(m_Music, loops);
	}

	//Pauses the music that's currently playing.
	void Music::Pause()
	{
		Mix_PauseMusic();
	}

	//Stops the music that's currently playing.
	void Music::Stop()
	{
		Mix_HaltMusic();
	}

	//Resumes the music that's currently playing.
	void Music::Resume()
	{
		Mix_ResumeMusic();
	}
}