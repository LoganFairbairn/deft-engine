#pragma once
#include <unordered_map>		//Allows use of unordered maps.
#include <glm/glm.hpp>			//Allows use of glm.
#include "Vector.h"

namespace Deft
{
	//This class is essentially a wrapper class for low level code.
	//Use this framework class to get input from the player.
	//This input class doens't do anything with the input, it simply recieves the input.
	class Input
	{
	public:
		Input() {};
		~Input() {};

		void Update();

		void PressKey(unsigned int keyID);
		void ReleaseKey(unsigned int keyID);

		bool IsKeyDown(unsigned int keyID);
		bool IsKeyPressed(unsigned int keyID);
		bool KeyReleased(unsigned int keyID);

	private:
		bool WasKeyDown(unsigned int keyID);

		std::unordered_map<unsigned int, bool> m_KeyMap;				//A list of keys that are currently pressed.
		std::unordered_map<unsigned int, bool> m_PreviousKeyMap;		//A list of keys that were previously pressed.
		std::unordered_map<unsigned int, bool> m_ReleasedKeyMap;		//A list of keys that were released this frame.
	};
}

