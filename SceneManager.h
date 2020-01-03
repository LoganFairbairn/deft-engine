#pragma once
#include "Messenger.h"

namespace Deft
{
	//The scene manager is responsible for...
	//1. Knowing what to load for a specific scene (scenes: main menu, game, editor, into).
	//2. Knowing what to unload for a specific scene.
	class SceneManager : Messenger
	{
	public:
		SceneManager();
		~SceneManager();
	};
}

