#include "UIWindow.h"

namespace Deft 
{
	UIWindow::UIWindow(UISystem* system, Vec2 position, Vec2 size)
	{
		SetAutoDraw(false);				//Windows never auto draw due to them needing to use glScissor functions.

		SetPosition(position);
		SetOriginalPosition(position);
		SetPreviousPosition(position);
		SetSize(size);

		SetDepth(-0.8f);	//Default window depth is 0.8f.
		SetColor(ColorRGBA8(0, 0, 0, 150));
		SetBarColor(ColorRGBA8(83, 83, 83, 255));
		SetName("Window Name");

		m_BarPadding = 5.0f;
		m_Padding = 5.0f;
		m_BarFont = ResourceManager::GetDefaultFont();
		m_BarHeight = 30.0f;

		AddCloseButton(system);

		m_PreviousPosition = m_Position;
	}

	void UIWindow::Update(Vec2 mousePosition)
	{
		//If the close button is pressed, hide the window.
		if (m_CloseButton->GetPressed())
		{
			HideWindow();
			m_CloseButton->SetPressed(false);
			return;
		}

		Vec2 topLeft = Vec2(m_Position.x, m_Position.y + m_Size.y);
		Vec2 bottomRight = Vec2(m_Position.x + m_Size.x, m_Position.y);

		if (IsInBox(mousePosition, topLeft, bottomRight))
		{
			SetMouseIsOver(true);
		}

		else
		{
			SetMouseIsOver(false);
		}

		MoveWindowObjects();

		//If the window is not visible, none of the objects in it should be visible either.
		if (GetVisible() == false)
		{
			for (unsigned int i = 0; i < m_UIObjects.size(); i++)
			{
				m_UIObjects[i]->SetVisible(false);
			}
		}

		//If the window is selected, change the bar color and adjust the depth so the window will appear above other windows.
		if (GetSelected())
		{
			SetBarColor(ColorRGBA8(51, 150, 242, 255));
			SetDepth(-0.85f);

			for (unsigned int i = 0; i < m_UIObjects.size(); i++)
			{

			}
		}

		else
		{
			SetBarColor(ColorRGBA8(83, 83, 83, 255));
			SetDepth(-0.8f);
		}
	}

	//Sets the depth of the window, then sets the depth of all objects inside the window to be above the window.
	void UIWindow::SetDepth(float depth)
	{
		m_Depth = depth;

		for (unsigned int i = 0; i < m_UIObjects.size(); i++)
		{
			m_UIObjects[i]->SetDepth(m_Depth - 0.01);
		}
	}

	//Shows the window and all of it's attatched objects.
	void UIWindow::ShowWindow()
	{
		SetVisible(true);

		for (unsigned int i = 0; i < m_UIObjects.size(); i++)
		{
			m_UIObjects[i]->SetVisible(true);
		}
	}

	//Hides the window and all of it's attatched objects.
	void UIWindow::HideWindow()
	{
		SetVisible(false);

		for (unsigned int i = 0; i < m_UIObjects.size(); i++)
		{
			m_UIObjects[i]->SetVisible(false);
		}
	}

	void UIWindow::Draw(Renderer* renderer)
	{
		//Set glScissor to the location of the UIWindow.
		glScissor(m_Position.x, m_Position.y, m_Size.x, m_Size.y + m_BarHeight);

		//Draw the window.
		renderer->DrawRectangle(m_Position, m_Size, m_Color, m_Depth);

		//Draw the Window bar and name.
		float x = m_Position.x;
		float y = m_Position.y + m_Size.y;
		renderer->DrawRectangle(Vec2(x, y), Vec2(m_Size.x, m_BarHeight), m_BarColor, m_Depth);

		x = m_Position.x + m_BarPadding;
		y = m_Position.y + m_Size.y + m_BarPadding;
		renderer->DrawText(m_BarFont, m_Name, Vec2(x, y), ColorRGBA8(255, 255, 255, 255), TextAlignment::LEFT, m_Scale, m_Depth - 0.01f);

		//If the window is selected add an outline around the window.
		if (m_Selected)
		{
			Vec2 size = Vec2(m_Size.x, m_Size.y + m_BarHeight);
			renderer->DrawRectangleOutline(m_Position, size, ColorRGBA8(200, 200, 200, 255), m_Depth);
		}

		//Draw all UIObjects attatched to this window.
		for (unsigned int i = 0; i < m_UIObjects.size(); i++)
		{
			if (m_UIObjects[i]->GetVisible())
			{
				m_UIObjects[i]->Draw(renderer);
			}
		}

		//Keep the window in screen.
		unsigned int cameraWidth = renderer->GetCameraWidth();
		unsigned int cameraHeight = renderer->GetCameraHeight();
		KeepWindowOnScreen(cameraWidth, cameraHeight);

		//Set Scissor back to the fullscreen.
		glScissor(0, 0, renderer->GetCameraWidth(), renderer->GetCameraHeight());
	}

	//Adds a object to the list of objects in the UIWindow.
	void UIWindow::AddObject(UIObject* object)
	{
		m_UIObjects.push_back(object);

		//Make the object not visible if the window isn't visible.
		if (GetVisible() == false)
		{
			object->SetVisible(false);
		}

		//Set the depth of the object so it appears over the window.
		object->SetDepth(m_Depth - 0.01f);

		//Make the object not automatically draw. The object will draw when the window is drawn.
		object->SetAutoDraw(false);

		Vec2 objectPosition = object->GetPosition();

		object->SetPosition(Vec2(objectPosition.x + m_Position.x, objectPosition.y + m_Position.y));
	}

	//Keeps the window on screen.
	void UIWindow::KeepWindowOnScreen(unsigned int cameraWidth, unsigned int cameraHeight)
	{
		if (m_Position.x < 0.0f)
		{
			m_Position.x = 0.0f;
		}

		if (m_Position.x + m_Size.x > cameraWidth)
		{
			m_Position.x = cameraWidth - m_Size.x;
		}

		if (m_Position.y < 0.0f)
		{
			m_Position.y = 0.0f;
		}

		if (m_Position.y > cameraHeight - m_Size.y)
		{
			m_Position.y = cameraHeight - m_Size.y;
		}
	}

	//Add a close button to the window.
	void UIWindow::AddCloseButton(UISystem* system)
	{
		Material closeButton = ResourceManager::GetMaterial("CloseButton");
		Material closeButtonOver = ResourceManager::GetMaterial("CloseButtonOver");
		float buttonWidth = closeButton.texture.width;
		float buttonHeight = closeButton.texture.height;

		float x = m_Size.x - buttonWidth;
		float y = m_Size.y + (m_BarHeight * 0.5f) - (buttonHeight * 0.5f);

		m_CloseButton = system->AddButton(Vec2(x, y));
		m_CloseButton->SetGraphics(closeButton, closeButtonOver);
		m_CloseButton->SetDescription("Closes this window.");

		AddObject(m_CloseButton);
	}

	//Moves all the objects in the window when the window is moved.
	void UIWindow::MoveWindowObjects()
	{
		if (m_PreviousPosition.x != m_Position.x || m_PreviousPosition.y != m_Position.y)
		{
			Vec2 distanceMoved;
			distanceMoved.x = m_Position.x - m_PreviousPosition.x;
			distanceMoved.y = m_Position.y - m_PreviousPosition.y;

			for (unsigned int i = 0; i < m_UIObjects.size(); i++)
			{
				Vec2 objectPosition = m_UIObjects[i]->GetPosition();

				objectPosition.x += distanceMoved.x;
				objectPosition.y += distanceMoved.y;

				m_UIObjects[i]->SetPosition(objectPosition);
			}

			m_PreviousPosition = m_Position;
		}
	}
}