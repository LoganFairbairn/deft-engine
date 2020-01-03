#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>	//Access to orthographic matrix functions.

namespace Deft
{
	//The 2D Camera allows the game to be made relative to a world space, or something else.
	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();

		void Initialize(int screenWidth, int screenHeight);
		void Update();
		glm::vec2 ScreenToWorldCoords(glm::vec2 screenPosition);
		bool IsBoxInView(float x, float y, float width, float height);
		bool IsBoxInView(const glm::vec2& position, const glm::vec2& dimensions);
		void SetPosition(glm::vec2 newPosition);
		void SetScale(float newScale);
		void SetSpeed(float newSpeed);
		void SetScaleSpeed(float newScaleSpeed);
		glm::vec2 GetPosition();
		glm::mat4 GetCameraMatrix();
		float GetScale();
		float GetSpeed();
		float GetScaleSpeed();

		int GetCameraWidth() const { return m_ScreenWidth; }
		int GetCameraHeight() const { return m_ScreenHeight; }

	private:
		int m_ScreenWidth;
		int m_ScreenHeight;
		float m_Scale;
		float m_Speed;
		float m_ScaleSpeed;
		float m_MinScale;
		float m_MaxScale;
		bool m_NeedsMatrixUpdate;
		glm::vec2 m_Position;
		glm::mat4 m_CameraMatrix;
		glm::mat4 m_OrthographicMatrix;
	};
}

