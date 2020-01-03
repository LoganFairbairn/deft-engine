#include "MessageBus.h"

namespace Deft
{
	MessageBus::MessageBus()
	{
		SetSystemNames();
		SetEventMessages();
	}

	//Stores a new reciever of messages in a list.
	void MessageBus::AddSystem(Systems systemID, System* system)
	{
		system->SetSystemID(systemID);

		m_Systems.insert(std::pair<Systems, System*>(systemID, system));
	}

	//Outputs a message (used for debugging).
	void MessageBus::LogMessage(const std::string& message)
	{
		std::cout << "\n" + message;
	}

	//Stores the message in a message que, to be sent to it's recipient when optimal. This helps keep frame rate high.
	void MessageBus::QueMessage(Message message)
	{
		m_EventQue.push_back(message);
	}

	//Sends an event from one game system to another instantly.
	void MessageBus::SendMessage(Message message)
	{
		//Send the message to it's intended recipient.
		Systems SystemID = message.GetSystemID();

		if (m_Systems[SystemID] == nullptr)
		{
			FatalError("Initialization error: Game system doesn't exist.");
		}

		else
		{
			//Send the event to the system reciever.
			m_Systems[SystemID]->HandleGameEvent(message.GetGameEventID());
			LogEvent(message);
		}
	}

	//Updates the message bus.
	//The message bus will hold non-prioritized events in a que if the game is running at a lower than desired FPS.
	void MessageBus::Update(float fps)
	{
		//Check to see if the FPS is high enough to output qued messages.
		//If the FPS is too low, the game will only output prioritized messages.
		if (fps > 50)
		{
			if (m_EventQue.size() > 0)
			{
				//Send the message to it's recipient.
				SendMessage(m_EventQue[0]);

				//Remove the message from the que.
				m_EventQue.erase(m_EventQue.begin());
			}
		}
	}

	//Logs a game event.
	void MessageBus::LogEvent(Message message)
	{
		if (m_EventMessages[message.GetGameEventID()] != "")
		{
			std::string output = "[" + m_SystemNames[message.GetSystemID()] + "]" + " recieved " + m_EventMessages[message.GetGameEventID()];
			LogMessage(output);
		}
	}

	//Sets system names (used for debugging).
	void MessageBus::SetSystemNames()
	{
		m_SystemNames[Systems::SYSTEM_MAIN] = "SYSTEM_MAIN";
		m_SystemNames[Systems::SYSTEM_RENDERER] = "SYSTEM_RENDERER";
		m_SystemNames[Systems::SYSTEM_INPUT] = "SYSTEM_INPUT";
		m_SystemNames[Systems::SYSTEM_AUDIO] = "SYSTEM_AUDIO";
		m_SystemNames[Systems::SYSTEM_IO] = "SYSTEM_IO";
		m_SystemNames[Systems::SYSTEM_UI] = "SYSTEM_UI";
	}

	//Define event messages for debugging.
	void MessageBus::SetEventMessages() 
	{
		m_EventMessages[GameEvents::LEFT_MOUSE_BUTTON_DOWN] = "";
		m_EventMessages[GameEvents::LEFT_MOUSE_BUTTON_PRESSED] = "Pressed left mouse button.";
		m_EventMessages[GameEvents::LEFT_MOUSE_BUTTON_RELEASED] = "Left mouse button released.";
		m_EventMessages[GameEvents::RIGHT_MOUSE_BUTTON_DOWN] = "";
		m_EventMessages[GameEvents::RIGHT_MOUSE_BUTTON_PRESSED] = "Right mouse button pressed.";
		m_EventMessages[GameEvents::RIGHT_MOUSE_BUTTON_RELEASED] = "Right mouse button released.";
		m_EventMessages[GameEvents::MIDDLE_MOUSE_BUTTON_DOWN] = "";
		m_EventMessages[GameEvents::MIDDLE_MOUSE_BUTTON_PRESSED] = "Middle mouse button pressed.";
		m_EventMessages[GameEvents::MIDDLE_MOUSE_BUTTON_RELEASED] = "Middle mouse button released.";

		m_EventMessages[GameEvents::OPEN_GAME_EDITOR] = "Open game editor.";
	}
}