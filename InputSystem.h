#pragma once
#include <unordered_map>
#include <glm/glm.hpp>
#include <SDL\SDL.h>
#include <string>

#include "System.h"
#include "Input.h"
#include "MessageBus.h"
#include "Vector.h"
#include "GameEvents.h"

namespace Deft
{
	class MessageBus;

	//Handles what to do with user input.
	//Example: Pressing "I" opens the inventory.
	class InputSystem : System
	{
	public:
		InputSystem(MessageBus* MessageBus);
		~InputSystem();

		void HandleGameEvent(GameEvents gameEvent);
		void Update();

		void HandleInput();

		//Gets the position of the mouse.
		Vec2 GetMousePosition() { return m_MousePosition; }

	private:
		MessageBus* m_MessageBus;
		Input* m_Input;
		Vec2 m_MousePosition;
	};
}

