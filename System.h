#pragma once
#include <string>
#include <memory>
#include "GameEvents.h"
#include "Systems.h"

namespace Deft
{
	//Any object that inherits from this class can send and recieve game events through the MessageBus.
	class System
	{
	public:
		System() {};
		~System() {};

		virtual void HandleGameEvent(GameEvents gameEvent) = 0;		//All game System must handle game events.

		void SetSystemID(Systems System) { m_SystemID = System; }

		Systems GetSystemID() { return m_SystemID; }

	protected:
		Systems m_SystemID;		//The enum ID for this system.
	};
}

