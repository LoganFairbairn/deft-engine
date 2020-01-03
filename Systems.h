#pragma once

namespace Deft
{
	//This is a list of Systems that can send and recieve game events which trigger different actions in the game.
	enum Systems
	{
		SYSTEM_MAIN = 0,
		SYSTEM_RENDERER,
		SYSTEM_UI,
		SYSTEM_INPUT,
		SYSTEM_AUDIO,
		SYSTEM_IO
	};
}