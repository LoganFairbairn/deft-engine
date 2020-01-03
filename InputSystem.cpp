#include "InputSystem.h"

namespace Deft
{
	InputSystem::InputSystem(MessageBus* MessageBus)
	{
		//Use the input framework.
		m_Input = new Input();

		//Set the message bus to commuicate with other System.
		m_MessageBus = MessageBus;
	}

	InputSystem::~InputSystem()
	{
	}

	void InputSystem::HandleGameEvent(GameEvents gameEvent)
	{

	}

	//Updates the input system.
	void InputSystem::Update()
	{
		m_Input->Update();

		SDL_Event evnt;

		//Loop through and handle all input events.
		while (SDL_PollEvent(&evnt))
		{
			switch (evnt.type)
			{
				case SDL_QUIT:
				{

				}
				break;

				case SDL_MOUSEMOTION:
				{
					m_MousePosition = Vec2(evnt.motion.x, evnt.motion.y);
				}
				break;

				case SDL_KEYDOWN:
				{
					m_Input->PressKey(evnt.key.keysym.sym);
				}	
				break;

				case SDL_KEYUP:
				{
					m_Input->ReleaseKey(evnt.key.keysym.sym);
				}
				break;

				case SDL_MOUSEBUTTONDOWN:
				{
					m_Input->PressKey(evnt.button.button);
				}
				break;

				case SDL_MOUSEBUTTONUP:
				{
					m_Input->ReleaseKey(evnt.button.button);
				}
				break;
			}
		}

		HandleInput();
	}

	//Send input game events to any system that needs them.
	//Note: The order in which these events are handled may matter in some instances.
	void InputSystem::HandleInput()
	{
		if (m_Input->IsKeyPressed(SDL_BUTTON_LEFT))
		{
			m_MessageBus->SendMessage(Message(Systems::SYSTEM_UI, GameEvents::LEFT_MOUSE_BUTTON_PRESSED));
		}

		if (m_Input->IsKeyDown(SDL_BUTTON_LEFT))
		{
			m_MessageBus->SendMessage(Message(Systems::SYSTEM_UI, GameEvents::LEFT_MOUSE_BUTTON_DOWN));
		}

		if (m_Input->IsKeyPressed(SDL_BUTTON_RIGHT))
		{
			m_MessageBus->SendMessage(Message(Systems::SYSTEM_UI, GameEvents::RIGHT_MOUSE_BUTTON_PRESSED));
		}

		if (m_Input->IsKeyDown(SDL_BUTTON_RIGHT))
		{
			m_MessageBus->SendMessage(Message(Systems::SYSTEM_UI, GameEvents::RIGHT_MOUSE_BUTTON_DOWN));
		}

		if (m_Input->KeyReleased(SDL_BUTTON_RIGHT) == true)
		{
			m_MessageBus->SendMessage(Message(Systems::SYSTEM_UI, GameEvents::RIGHT_MOUSE_BUTTON_RELEASED));
		}

		if (m_Input->IsKeyPressed(SDLK_ESCAPE))
		{
			m_MessageBus->SendMessage(Message(Systems::SYSTEM_MAIN, GameEvents::QUIT_GAME));
		}
	}
}

