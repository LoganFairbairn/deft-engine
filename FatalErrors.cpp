#include "FatalErrors.h"
#include <cstdlib>
#include <iostream>
#include <SDL/SDL.h>

namespace Deft
{
	//Creates an error with a custom message that crashes the game.
	void FatalError(std::string errorString)
	{
		std::cout << "\n" + errorString << std::endl;
		std::cout << "Enter any key to quit...";
		int tmp;
		std::cin >> tmp;

		//Quit all of the SDL functionality.
		SDL_Quit();

		//Exit the application and send an error number.
		exit(69);
	}
}