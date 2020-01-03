#pragma once

namespace Deft
{
	enum RendererDrawMode
	{
		DRAW_MODE_GAME = 0,		//Use this mode for drawing the game.
		DRAW_MODE_UI,			//Use this mode for drawing the UI.
		DRAW_MODE_UI_WINDOWS	//Use this mode for drawing UI windows (cursor is drawn here too).
	};
}