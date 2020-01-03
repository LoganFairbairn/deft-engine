#include "Message.h"

namespace Deft
{
	Message::Message(Systems SystemID, GameEvents gameEventID)
	{
		m_SystemID = SystemID;
		m_GameEventID = gameEventID;
	}
}