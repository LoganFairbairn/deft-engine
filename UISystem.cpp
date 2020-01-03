#include "UISystem.h"

namespace Deft
{
	UISystem::UISystem(MessageBus* MessageBus)
	{
		m_MessageBus = MessageBus;
	}

	//Handles game events send to this game system.
	void UISystem::HandleGameEvent(GameEvents gameEvent)
	{
		if (gameEvent == GameEvents::LEFT_MOUSE_BUTTON_PRESSED)
		{
			HandleButtonLeftPressed();
		}

		if (gameEvent == GameEvents::RIGHT_MOUSE_BUTTON_PRESSED)
		{
			//Hold windows if they are right clicked on.
			for (auto& i : m_UIWindows)
			{
				if (i.second->GetVisible())
				{
					if (i.second->GetMouseIsOver())
					{
						Vec2 mousePosition = m_Cursor.GetPosition();
						Vec2 windowPosition = i.second->GetPosition();
						float x = windowPosition.x - mousePosition.x;
						float y = windowPosition.y - mousePosition.y;

						i.second->SetMouseGrabOffSet(Vec2(x, y));
						i.second->SetHeld(true);

						SetSelectedObject(i.second.get());
					}
				}
			}
		}

		if (gameEvent == GameEvents::RIGHT_MOUSE_BUTTON_DOWN)
		{
			//Move windows relative to the cursor when they are held.
			for (auto& i : m_UIWindows)
			{
				if (i.second->GetVisible())
				{
					if (i.second->GetMouseIsOver() && i.second->GetHeld())
					{
						Vec2 mousePosition = m_Cursor.GetPosition();
						Vec2 grabOffSet = i.second->GetMouseGrabOffSet();
						float x = mousePosition.x + grabOffSet.x;
						float y = mousePosition.y + grabOffSet.y;

						i.second->SetPosition(Vec2(x, y));
					}
				}
			}
		}

		if (gameEvent == GameEvents::RIGHT_MOUSE_BUTTON_RELEASED)
		{
			for (auto& i : m_UIWindows)
			{
				if (i.second->GetVisible())
				{
					i.second->SetMouseGrabOffSet(Vec2(0.0f, 0.0f));
					i.second->SetHeld(false);
				}
			}
		}
	}

	//Updates the user interface.
	void UISystem::Update(Vec2 mousePosition)
	{
		m_Cursor.SetPosition(mousePosition);

		//Update all visible UI objects.
		for (unsigned int i = 0; i < m_UIObjects.size(); i++)
		{
			if (m_UIObjects[i]->GetVisible() == true)
			{
				m_UIObjects[i]->Update(mousePosition);
			}
		}
	}

	//Draws UI objects if they are visible, and auto-draw is turned on.
	void UISystem::Draw(Renderer* renderer)
	{
		Vec2 cursorPosition = m_Cursor.GetPosition();

		for (unsigned int i = 0; i < m_UIObjects.size(); i++)
		{
			//Only draw the object if they are visible.
			if (m_UIObjects[i]->GetVisible())
			{
				//Draw the object here if it's set to automatically draw.
				if (m_UIObjects[i]->GetAutoDraw())
				{
					m_UIObjects[i]->Draw(renderer);
				}
			
				//Draw info boxes for the objects if they have a set descrioption.
				std::string description = m_UIObjects[i]->GetDescription();

				if (m_UIObjects[i]->GetMouseIsOver() && description != "")
				{
					renderer->DrawInfoBox(cursorPosition, 40.0f, description, ResourceManager::GetDefaultFont());
				}
			}
		}
	}

	//Draws all of the UIWindows.
	void UISystem::DrawWindows(Renderer* renderer)
	{
		for (auto& i : m_UIWindows)
		{
			//Only draw the window if it's visible.
			if (i.second->GetVisible())
			{
				i.second->Draw(renderer);
			}
		}
	}

	//Draws the cursor to the screen.
	void UISystem::DrawCursor(Renderer* renderer)
	{
		Vec2 cursorPosition = m_Cursor.GetPosition();
		renderer->DrawSprite(m_Cursor.GetMaterial(), cursorPosition, 0.0f, m_Cursor.GetPivot(), ColorRGBA8(255, 255, 255, 255), -1.0f);
	}

	//Adds a window to the UI.
	UIWindow* UISystem::AddWindow(Vec2 position, Vec2 size, const std::string& windowName)
	{
		//m_MaterialCache.insert(make_pair(materialName, material));
		m_UIWindows.insert(make_pair(windowName, std::make_shared<UIWindow>(this, position, size)));
		m_UIWindows.at(windowName)->SetName(windowName);

		m_UIObjects.push_back((std::shared_ptr<UIObject>)m_UIWindows.at(windowName));
		m_UIObjects.back()->SetID(m_UIObjects.size());

		return m_UIWindows.at(windowName).get();
	}

	//Adds a UI Text object at the given position.
	UIText* UISystem::AddText(Vec2 position, const std::string& text)
	{
		m_UITexts.push_back(std::make_shared<UIText>());
		m_UITexts.back()->SetText(text);
		m_UITexts.back()->SetPosition(position);
		m_UITexts.back()->SetOriginalPosition(position);

		m_UIObjects.push_back((std::shared_ptr<UIObject>)m_UITexts.back());
		m_UIObjects.back()->SetID(m_UIObjects.size());

		return m_UITexts.back().get();
	}

	//Adds a button to the UI.
	UIButton* UISystem::AddButton(Vec2 position)
	{
		m_UIButtons.push_back(std::make_shared<UIButton>());
		m_UIButtons.back()->SetPosition(position);
		m_UIButtons.back()->SetOriginalPosition(position);

		m_UIObjects.push_back((std::shared_ptr<UIObject>)m_UIButtons.back());
		m_UIObjects.back()->SetID(m_UIObjects.size());

		return m_UIButtons.back().get();
	}

	//Adds an image to the UI.
	UIImage* UISystem::AddImage(Vec2 position, Material material)
	{
		m_UIImages.push_back(std::make_shared<UIImage>());
		m_UIImages.back()->SetImage(material);
		m_UIImages.back()->SetPosition(position);
		m_UIImages.back()->SetOriginalPosition(position);

		m_UIObjects.push_back((std::shared_ptr<UIImage>)m_UIImages.back());
		m_UIObjects.back()->SetID(m_UIObjects.size());

		return m_UIImages.back().get();
	}

	//Adds a UIDropDown to the UI.
	UIDropDown* UISystem::AddDropDown(Vec2 position, const std::string& dropDownName)
	{
		m_UIDropDowns.push_back(std::make_shared<UIDropDown>(this));
		m_UIDropDowns.back()->SetText(dropDownName);
		m_UIDropDowns.back()->SetPosition(position);
		m_UIDropDowns.back()->SetOriginalPosition(position);

		m_UIObjects.push_back((std::shared_ptr<UIObject>)m_UIDropDowns.back());
		m_UIObjects.back()->SetID(m_UIObjects.size());

		return m_UIDropDowns.back().get();
	}

	//Hides the given UI group.
	void UISystem::ShowGroup(UIGroup group)
	{
		for (unsigned int i = 0; i < m_UIObjects.size(); i++)
		{
			if (m_UIObjects[i]->GetGroup() == group)
			{
				m_UIObjects[i]->SetVisible(true);
			}
		}
	}

	//Shows the given UI group.
	void UISystem::HideGroup(UIGroup group)
	{
		for (unsigned int i = 0; i < m_UIObjects.size(); i++)
		{
			if (m_UIObjects[i]->GetGroup() == group)
			{
				m_UIObjects[i]->SetVisible(false);
			}
		}
	}

	//Shows the window with the given name if one exists.
	void UISystem::ShowWindow(const std::string& windowName)
	{
		auto i = m_UIWindows.find(windowName);

		if (i != m_UIWindows.end())
		{
			i->second->ShowWindow();
		}
	}

	//Hides a window with the given name if one exists.
	void UISystem::HideWindow(const std::string& windowName)
	{
		auto i = m_UIWindows.find(windowName);

		if (i != m_UIWindows.end())
		{
			i->second->HideWindow();
		}
	}

	//Sets the cursor for the game.
	void UISystem::SetCursor(Material material, Vec2 pivot)
	{
		m_Cursor.SetMaterial(material);
		m_Cursor.SetPivot(pivot);
	}

	//Deselectes all objects, then selected the given object.
	void UISystem::SetSelectedObject(UIObject* object)
	{
		for (unsigned int i = 0; i < m_UIObjects.size(); i++)
		{
			m_UIObjects[i]->SetSelected(false); 
		}

		object->SetSelected(true);
	}

	//Does stuff when the left button is pressed.
	void UISystem::HandleButtonLeftPressed()
	{
		//If any windows are left clicked, set them to selected.
		for (auto& i : m_UIWindows)
		{
			if (i.second->GetVisible())
			{
				if (i.second->GetMouseIsOver())
				{
					SetSelectedObject(i.second.get());
				}
			}
		}

		//If any visible DropDowns are left clicked, set them pressed.
		for (unsigned int i = 0; i < m_UIDropDowns.size(); i++)
		{
			if (m_UIDropDowns[i]->GetVisible())
			{
				if (m_UIDropDowns[i]->GetMouseIsOver())
				{
					m_UIDropDowns[i]->SetPressed(true);
				}
			}
		}

		//If any visible buttons are left clicked, set them to pressed.
		for (unsigned int i = 0; i < m_UIButtons.size(); i++)
		{
			if (m_UIButtons[i]->GetVisible())
			{
				if (m_UIButtons[i]->GetMouseIsOver())
				{
					//Set the button pressed.
					m_UIButtons[i]->SetPressed(true);

					//Do something based on the button text.
					if (m_UIButtons[i]->GetText() == "QUIT GAME")
					{
						m_MessageBus->SendMessage(Message(Systems::SYSTEM_MAIN, GameEvents::QUIT_GAME));
						m_UIButtons[i]->SetPressed(false);
					}

					if (m_UIButtons[i]->GetText() == "GAME EDITOR")
					{
						m_MessageBus->SendMessage(Message(Systems::SYSTEM_MAIN, GameEvents::OPEN_GAME_EDITOR));
						m_UIButtons[i]->SetPressed(false);
					}
				}
			}
		}
	}
}