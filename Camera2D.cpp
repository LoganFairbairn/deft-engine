#include "Camera2D.h"

namespace Deft
{
	Camera2D::Camera2D() : m_ScreenWidth(500), 
		m_ScreenHeight(500), 
		m_Scale(1.0f), 
		m_Speed(1.0f), 
		m_ScaleSpeed(1.0f),
		m_MinScale(0.25f),
		m_MaxScale(3.0f),
		m_NeedsMatrixUpdate(true), 
		m_Position(0.0f, 0.0f), 
		m_CameraMatrix(1.0f), 
		m_OrthographicMatrix(1.0f)
	{

	}

	Camera2D::~Camera2D()
	{

	}

	//Sets up the camera by providing it with enough informtion to use matrix transformations.
	void Camera2D::Initialize(int screenWidth, int screenHeight)
	{
		m_ScreenWidth = screenWidth;
		m_ScreenHeight = screenHeight;

		//Center the camera around the 0,0 point.
		SetPosition(glm::vec2(m_ScreenWidth * 0.5f, m_ScreenHeight * 0.5f));

		//Build the orthographic matrix whenever the screen width or the screen height changes.
		m_OrthographicMatrix = glm::ortho(0.0f, (float)m_ScreenWidth, 0.0f, (float)m_ScreenHeight);
	}

	//Converts screen coordinates (coordinates relative to screen resolution) to world coordinates (coordinates relative to camera).
	glm::vec2 Camera2D::ScreenToWorldCoords(glm::vec2 screenPosition)
	{
		//Invert the Y direction.
		screenPosition.y = m_ScreenHeight - screenPosition.y;
		
		//Make 0 the center.
		screenPosition -= glm::vec2(m_ScreenWidth * 0.5f, m_ScreenHeight * 0.5f);
		
		//Scale the screen coordinates.
		screenPosition /= m_Scale;

		//Translate with the camera position.
		screenPosition += m_Position;

		//Return the newly converted world coordinates.
		return screenPosition;
	}

	//Checks to see if a box is within view of the camera. This is used mostly for optimization (don't draw what isn't on screen!).
	bool Camera2D::IsBoxInView(float x, float y, float width, float height)
	{
		glm::vec2 position = glm::vec2(x, y);
		glm::vec2 dimensions = glm::vec2(width, height);

		return IsBoxInView(position, dimensions);
	}

	//Checks to see if a box is within view of the camera. This is used mostly for optimization (don't draw what isn't on screen!).
	bool Camera2D::IsBoxInView(const glm::vec2& position, const glm::vec2& dimensions)
	{
		//Scale the screen dimensions by the cameras scale before doing any calculations.
		glm::vec2 scaledScreenDimensions = glm::vec2(m_ScreenWidth, m_ScreenHeight) / m_Scale;

		//This is the minimum distance 
		const float MIN_DISTANCE_X = dimensions.x / 2.0f + scaledScreenDimensions.x / 2.0f;
		const float MIN_DISTANCE_Y = dimensions.y / 2.0f + scaledScreenDimensions.y / 2.0f;

		//Center positon of parameters.
		glm::vec2 centerPosition = position + dimensions * 0.5f;

		//Center position of the camera.
		glm::vec2 centerCameraPosition = m_Position;

		//Vector from the input to the camera.
		glm::vec2 distanceVector = centerPosition - centerCameraPosition;

		float xDepth = MIN_DISTANCE_X - abs(distanceVector.x);
		float yDepth = MIN_DISTANCE_Y - abs(distanceVector.y);

		//If this is true, the object is colliding.
		if (xDepth > 0 && yDepth > 0)
		{
			//There was a collison.
			return true;
		}

		return false;
	}

	//Updates the camera movemen
	void Camera2D::Update()
	{
		//Only update the matrix when you need to, this saves a little bit of processing power.
		if (m_NeedsMatrixUpdate)
		{

			//Note: It does matter which order you translate and scale the camera matrix in. Always translate, then scale. Doing them in the wrong order will cause a lot of graphical bugs.
			//Set up a translation to move the camera.
			//The translation is negative to give the effect the camera is moving around the virtual world by moving everything in the opposite direction of the camera, then translate the camera matrix.
			glm::vec3 translate(-m_Position.x + (m_ScreenWidth * 0.5f), -m_Position.y + (m_ScreenHeight * 0.5f), 0.0f);
			m_CameraMatrix = glm::translate(m_OrthographicMatrix, translate);

			//Set up a scale to adjust the camera matrix using the cameras scale member variable, then apply it.
			glm::vec3 scale(m_Scale, m_Scale, 0.0f);
			m_CameraMatrix = glm::scale(glm::mat4(1.0f), scale) * m_CameraMatrix;

			//After the camera is finished recalculating it's matrices, we can postpone recalculating them until required.
			m_NeedsMatrixUpdate = false;
		}

		//Stop the camera from having a negative scale.
		if (m_Scale < m_MinScale)
		{
			m_Scale = m_MinScale;
		}
		
		//Stop the camera from having a super large scale.
		if (m_Scale > m_MaxScale)
		{
			m_Scale = m_MaxScale;
		}
	}

	void Camera2D::SetPosition(glm::vec2 newPosition)
	{
		m_Position = newPosition;

		//When the camera's position is changed, it will need to recalculate it's matrices.
		m_NeedsMatrixUpdate = true;
	}

	void Camera2D::SetScale(float newScale)
	{
		m_Scale = newScale;
		
		//When the camera's scale is changed, it will need to recalculate it's matrices.
		m_NeedsMatrixUpdate = true;
	}

	void Camera2D::SetSpeed(float newSpeed)
	{
		m_Speed = newSpeed;
	}

	void Camera2D::SetScaleSpeed(float newScaleSpeed)
	{
		m_ScaleSpeed = newScaleSpeed;
	}

	glm::vec2 Camera2D::GetPosition()
	{
		return m_Position;
	}

	glm::mat4 Camera2D::GetCameraMatrix()
	{
		return m_CameraMatrix;
	}

	float Camera2D::GetScale()
	{
		return m_Scale;
	}

	float Camera2D::GetSpeed()
	{
		return m_Speed;
	}

	float Camera2D::GetScaleSpeed()
	{
		return m_ScaleSpeed;
	}
}