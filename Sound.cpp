#include "Sound.h"

namespace Deft
{
	//Plays the specified sound effect.
	void Sound::Play(int loops)
	{
		if (Mix_PlayChannel(-1, m_Chunk, loops) == -1)
		{
			//If the audio fails to play, it might be out of available audio channels.
			//Play the audio on the first channel instead.
			if (Mix_PlayChannel(0, m_Chunk, loops) == -1)
			{
				FatalError("Mix_PlayChannel error: " + std::string(Mix_GetError()));
			}
		}
	}
}