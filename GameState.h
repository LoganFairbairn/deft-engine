#pragma once

namespace Deft
{
	//This is the current state of the game.
	enum GameState
	{
		GAMESTATE_LOADING,
		GAMESTATE_MENU,
		GAMESTATE_EDITOR,
		GAMESTATE_PLAYING,
		GAMESTATE_EXIT
	};
}