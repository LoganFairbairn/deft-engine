#include "AudioSystem.h"

namespace Deft
{
	AudioSystem::AudioSystem(MessageBus* MessageBus)
	{
		m_MessageBus = MessageBus;
	}

	void AudioSystem::HandleGameEvent(GameEvents gameEvent)
	{
	}
}
