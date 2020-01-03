#pragma once
#include <string>
#include <SDL/SDL_mixer.h>
#include "MessageBus.h"
#include "System.h"
#include "ResourceManager.h"		//Allows access to game's resource caches

namespace Deft
{
	//The AudioManager is responsible for...
	//1. Loading & unloading music and sounds.
	//2. Playing music and sounds when a specific function message is recieved.
	class AudioSystem : System
	{
	public:
		AudioSystem(MessageBus* MessageBus);
		~AudioSystem() {};

		void HandleGameEvent(GameEvents gameEvent);

	private:
		MessageBus* m_MessageBus;
	};
}

