#pragma once
#include <string>
#include "System.h"
#include "GameEvents.h"

namespace Deft
{
	//This is a message that can be sent from one system in the game engine to another using the message bus.
	//Generally this message will be something like "OPEN_INVENTORY".
	class Message
	{
	public:
		Message() {};
		Message(Systems SystemID, GameEvents gameEventID);
		~Message() {};

		void SetGameEventID(GameEvents gameEvent) { m_GameEventID = gameEvent; }
		void SetSystemID(Systems SystemID) { m_SystemID = SystemID; }

		GameEvents GetGameEventID() { return m_GameEventID; }
		Systems GetSystemID() { return m_SystemID; }

	private:
		Systems m_SystemID;				//The ID of the system you want to send to.
		GameEvents m_GameEventID;		//A game event ID enum.
	};
}