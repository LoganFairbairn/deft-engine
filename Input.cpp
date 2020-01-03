#include "Input.h"

namespace Deft
{
	void Input::Update()
	{
		//Record the keys that were just pressed by looping through our key states list and copy them to the previous key states list.
		for (auto& i : m_KeyMap)
		{
			m_PreviousKeyMap[i.first] = i.second;
		}

		//Set all of the released keys to false (no keys were released this frame.)
		for (auto& i : m_ReleasedKeyMap)
		{
			m_ReleasedKeyMap[i.first] = false;
		}
	}

	//Puts a pressed key into the key map.
	void Input::PressKey(unsigned int keyID)
	{
		m_KeyMap[keyID] = true;
	}

	//Takes a pressed key out of the key map.
	void Input::ReleaseKey(unsigned int keyID)
	{
		m_KeyMap[keyID] = false;

		m_ReleasedKeyMap[keyID] = true;
	}

	//Checks to see if the given key is pressed down.
	bool Input::IsKeyDown(unsigned int keyID)
	{
		//The key ID will not exist until the player pressed that key once, therefore checking to see if the key exists within the key map first is required.
		//Cycle through the key map and look to see if they key exists and return wether the key is pressed or not if it does exist.
		auto i = m_KeyMap.find(keyID);
		if (i != m_KeyMap.end())
		{
			return i->second;
		}

		//Return false by default.
		else
		{
			return false;
		}
	}

	//Checks to see if a key was just pressed this frame.
	bool Input::IsKeyPressed(unsigned int keyID)
	{
		//Is the key is pressed this frame, but wasn't pressed last frame, return true.
		if (IsKeyDown(keyID) == true && WasKeyDown(keyID) == false)
		{
			return true;
		}

		else
		{
			return false;
		}
	}

	//If the key was released this frame, return true.
	bool Input::KeyReleased(unsigned int keyID)
	{
		//If the key is in the released key map, it was released this frame.
		auto i = m_ReleasedKeyMap.find(keyID);
		if (i != m_ReleasedKeyMap.end())
		{
			return i->second;
		}
		
		else
		{
			false;
		}
	}

	//Checks to see if the given key was pressed down the previous frame.
	bool Input::WasKeyDown(unsigned int keyID)
	{
		//The key ID will not exist until the player pressed that key once, therefore checking to see if the key exists within the key map first is required.
		//Cycle through the key map and look to see if they key exists and return wether the key is pressed or not if it does exist.
		auto i = m_PreviousKeyMap.find(keyID);
		if (i != m_PreviousKeyMap.end())
		{
			return i->second;
		}

		//Return false by default.
		else
		{
			return false;
		}
	}
}
