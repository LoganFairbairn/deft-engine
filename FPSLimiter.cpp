#pragma once
#include <SDL/SDL.h>

#include "FPSLimiter.h"

namespace Deft
{
	FPSLimiter::FPSLimiter()
	{
		//Empty.
	}

	//Initializes the FPS limiter.
	void FPSLimiter::Initialize(int maxFPS)
	{
		SetMaxFPS(maxFPS);
	}

	//Sets the maximum FPS.
	void FPSLimiter::SetMaxFPS(int maxFPS)
	{
		m_MaxFPS = maxFPS;
	}

	//Begins the FPS limiter, call this at the begining of a game loop.
	void FPSLimiter::Begin()
	{
		//Stores the number of ms since SDL was initialized.
		m_StartTicks = SDL_GetTicks();
	}

	//Calculates and returns the current frames per second.
	float FPSLimiter::End()
	{
		//Calculate the FPS.
		CalculateFPS();
		
		//Limit the FPS.
		float frameTicks = (float)SDL_GetTicks() - m_StartTicks;
		if (1000.0f / m_MaxFPS > frameTicks)
		{
			//Note that SDL_Delay on some windows machines must delay for a minimum of 16ms.
			SDL_Delay(1000.0f / m_MaxFPS - frameTicks);
		}
		
		//Return the number of frames per second.
		return m_FPS;
	}

	//Calculates the amount of frames that are rendered per second in the main game loop.
	//Note: This is the true FPS value, some computer monitors have a limit to how many frames they can render per second (usually 60).
	//Note: Some graphics cards have settings that interfere and throttle the FPS.
	//Note: VSync can cause a large dip in FPS, or throttle the FPS depending on the graphics card and it's settings.
	void FPSLimiter::CalculateFPS()
	{
		//How many frames that will be sampled from to form the average FPS.
		static const int FRAME_SAMPLES = 28;
		static float frameSampleBuffer[FRAME_SAMPLES];
		static int currentFrameOfBuffer = 0;

		//SDL get ticks returns the number of ms since SDL was initialized.
		static float previousTicks = SDL_GetTicks();
		float currentTicks = SDL_GetTicks();

		//The first execution of this code current ticks and previous ticks should be the same.
		m_FrameTime = currentTicks - previousTicks;

		//Frame time will be stored in next buffer slot in order, when it reaches the end of the buffer slot it will
		//restart at the begining of the buffer.
		frameSampleBuffer[currentFrameOfBuffer % FRAME_SAMPLES] = m_FrameTime;

		//Reset previous ticks.
		previousTicks = currentTicks;

		//Increment current frame (incrementing here to avoid a divide by 0).
		currentFrameOfBuffer++;

		//Only calculate the average frame time from filled samples in the buffer.
		int count;
		if (currentFrameOfBuffer < FRAME_SAMPLES)
		{
			count = currentFrameOfBuffer;
		}

		else
		{
			count = FRAME_SAMPLES;
		}

		//Calculate the average frame time based on the information in the frame buffer.
		float frameTimeAverage = 0;
		for (int i = 0; i < count; i++)
		{
			frameTimeAverage += frameSampleBuffer[i];
		}

		frameTimeAverage /= count;

		//Avoid division by 0.
		if (frameTimeAverage > 0)
		{
			//Calculate the frames per second (1000 ms in 1 second).
			m_FPS = 1000.0f / frameTimeAverage;
		}

		//If the average frame time is 0, just show 60 FPS.
		else
		{
			m_FPS = 60.0f;
		}
	}
}