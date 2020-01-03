#pragma once#
#include <string>

#include "GL/glew.h"

namespace Deft
{
	//The LoadingScreen is drawn while the game loads.
	class LoadingScreen
	{
	public:
		LoadingScreen();
		~LoadingScreen();

		void Initialize();
		void DrawLoadingScreen(float percentCompleted);

		void SetBackground(GLuint textureID);
		void SetGameLogo(GLuint textureID);

	private:
		GLuint m_Background;
		GLuint m_GameLogo;

		float m_LoadingCompletion = 0;			//This is a completion percent for loading.
		std::string m_FunnyLoadingText[20];
	};
}