#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include "FatalErrors.h"
#include "Message.h"
#include "GameEvents.h"
#include "System.h"
#include "HashString.h"

namespace Deft
{
	//The MessageBus works like a postal service for GameEvents.
	//MessageBus sends GameEvents between Systems, helping the engine stay modular.

	//Example:
	//1. Input recieve a key press event "I".
	//2. InputManager knows "I" is supposed to open the players inventory and sends the message "Open Inventory" to the MessageBus.
	//3. The MessageBus sends "Open Inventory" to all objects that require that message.
	//4. Other managers and the game recieves the message "Open Inventory" and preform their task involving that operation (if there is one).
	//The RenderManager renders the inventory to the screen.
	//The AudioManager plays a sound effect for opening the inventory.
	//The Game Logic pauses the game.
	class MessageBus
	{
	public:
		MessageBus();
		~MessageBus() {};

		void Update(float fps);

		void AddSystem(Systems SystemID, System* System);

		void LogMessage(const std::string& message);
		void QueMessage(Message message);
		void SendMessage(Message message);

	private:
		void LogEvent(Message gameEvent);
		void SetSystemNames();
		void SetEventMessages();

		std::map<Systems, System*> m_Systems;		//A pointer to every game system.
		std::vector<Message> m_EventQue;			//Que of messages waiting to go through.

		std::string m_SystemNames[6];
		std::string m_EventMessages[GameEvents::LAST_EVENT];
	};
}

