#pragma once

namespace Deft
{
	//These are events that can be sent between System to preform different tasks.
	//Example: The InputSystem sends OPEN_INVENTORY to the UISystem which then draws the inventory to the screen.
	//Note: Only include events that will be used somewhere, adding events here increases compile time.
	enum GameEvents
	{
		RIGHT_MOUSE_BUTTON_DOWN = 0,
		RIGHT_MOUSE_BUTTON_PRESSED,
		RIGHT_MOUSE_BUTTON_RELEASED,
		LEFT_MOUSE_BUTTON_DOWN,
		LEFT_MOUSE_BUTTON_PRESSED,
		LEFT_MOUSE_BUTTON_RELEASED,
		MIDDLE_MOUSE_BUTTON_PRESSED,
		MIDDLE_MOUSE_BUTTON_DOWN,
		MIDDLE_MOUSE_BUTTON_RELEASED,
		OPEN_MENU,
		OPEN_GAME_EDITOR,
		OPEN_INVENTORY,
		QUIT_GAME,
		LAST_EVENT
	};
}