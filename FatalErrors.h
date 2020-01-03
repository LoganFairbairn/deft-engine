#pragma once
#include <string>


namespace Deft
{
	//These are debug errors that cause the game to crash when something goes seriously wrong.
	extern void FatalError(std::string errorString);
}