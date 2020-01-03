#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <string>

namespace Deft
{
	//WindowsFlags is made an enum to allow combination using bit operators of all window states into a single variable when it's passed into CreateWindow.
	enum WindowFlags 
	{ 
		INVISIBLE = 0x1, 
		FULLSCREEN = 0x2,
		BOARDERLESS = 0x4 
	};

	//This is a basic window for the game.
	class Window
	{
	public:
		Window();
		~Window();

		int CreateWindow(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);
		void SwapWindowBuffer();
		int GetScreenWidth();
		int GetScreenHeight();

	private:
		SDL_Window* m_SDLWindow;
		int m_ScreenWidth;
		int m_ScreenHeight;
	};
}
