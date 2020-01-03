#include "Window.h"
#include "FatalErrors.h"

namespace Deft
{
	Window::Window()
	{
	}


	Window::~Window()
	{
	}

	//Creates an SDL window with specified parameters.
	int Window::CreateWindow(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags)
	{
		Uint32 flags = SDL_WINDOW_OPENGL;

		//Set up window states based on the flags that were passed in.
		if (currentFlags & INVISIBLE)
		{
			flags |= SDL_WINDOW_HIDDEN;
		}

		if (currentFlags & FULLSCREEN)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		if (currentFlags & BOARDERLESS)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		//Creates a window using SDL. This window is set to draw with openGL.
		m_SDLWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);

		//Gives an error if the window couldn't be created.
		if (m_SDLWindow == nullptr)
		{
			FatalError("SDL Window could not be created!");
		}

		//Before drawing OpenGL must be initialized using an OpenGL context, a state machine that stores all data related to rendering your application. 
		//When the application is closed, OpenGL context is destroyed and everything is cleared up.
		//Creates a OpenGL context and checks for any errors with it's creation.
		SDL_GLContext glContext = SDL_GL_CreateContext(m_SDLWindow);
		if (glContext == nullptr)
		{
			FatalError("SDL_GL context could not be created!");
		}

		//Initializes GLEW functionaliy, and checks for any errors with it's initialization.
		GLenum error = glewInit();
		if (error != GLEW_OK)
		{
			FatalError("Could not initialize GLEW");
		}

		//Prints out your current version of OpenGL. Old versions of OpenGL may have errors with this engine (especially when running versions of OpenGL older than 3.0).
		printf("OpenGL version supported by this platform (%s): ", glGetString(GL_VERSION));

		//Set the default game background color.
		glClearColor(0.75f, 0.75f, 0.75f, 1.0f);

		//Set vertical syncronization, the player should be able to toggle this from the game settings.
		//Vertical syncronization is off by default, as it can have issues on preformance by lowering the FPS by a large amount.
		SDL_GL_SetSwapInterval(0);

		//Enable OpenGL alpha blending (for transparency).
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Enable OpenGL Depth Buffer
		glEnable(GL_DEPTH_TEST);

		//Enable OpenGL Scissor test (for drawing to windows).
		glEnable(GL_SCISSOR_TEST);

		//Don't show cursor.
		SDL_ShowCursor(SDL_DISABLE);

		return 1;
	}

	//Swaps the window buffer when double buffering is on.
	void Window::SwapWindowBuffer()
	{
		SDL_GL_SwapWindow(m_SDLWindow);
	}

	//Returns the width of the screen.
	int Window::GetScreenWidth()
	{
		return m_ScreenWidth;
	}

	//Returns the height of the screen.
	int Window::GetScreenHeight()
	{
		return m_ScreenHeight;
	}
}