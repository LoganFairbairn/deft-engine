#pragma once

namespace Deft
{
	//This object is used for calculating and limiting the FPS of a game loop.
	class FPSLimiter
	{
	public:
		FPSLimiter();

		void Initialize(int maxFPS);
		void SetMaxFPS(int maxFPS);
		void Begin();
		float End();

	private:
		void CalculateFPS();

		unsigned int m_StartTicks;	//The number of ms since the start of the frame (updated at the start of the game loop).
		float m_FPS;
		unsigned int m_MaxFPS;
		float m_FrameTime;
	};
}