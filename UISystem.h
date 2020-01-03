#pragma once
#include <vector>
#include <memory>

#include "Renderer.h"
#include "System.h"
#include "MessageBus.h"
#include "MathFunctions.h"

//UI
#include "Cursor.h"
#include "UIButton.h"
#include "UIText.h"
#include "UIImage.h"
#include "UIDropDown.h"
#include "UIWindow.h"

namespace Deft
{
	class UIWindow;
	class UIDropDown; 

	//Add all objects to a list of UI objects, then call their draw functions.
	//Add UI objects using the Add functions, give them a group ID and toggle groups of UI objects using their group ID.
	class UISystem : System
	{
	public:
		UISystem(MessageBus* MessageBus);
		~UISystem() {};

		void HandleGameEvent(GameEvents gameEvent);

		void Update(Vec2 mousePosition);

		void Draw(Renderer* renderer);
		void DrawWindows(Renderer* renderer);
		void DrawCursor(Renderer* renderer);

		//Functions for adding UI objects.
		UIWindow* AddWindow(Vec2 position, Vec2 size, const std::string& windowName);
		UIText* AddText(Vec2 position, const std::string& text);
		UIButton* AddButton(Vec2 position);
		UIImage* AddImage(Vec2 position, Material material);
		UIDropDown* AddDropDown(Vec2 position, const std::string& dropDownName);

		void ShowGroup(UIGroup group);
		void HideGroup(UIGroup group);

		void ShowWindow(const std::string& windowName);
		void HideWindow(const std::string& windowName);

		//Setters.
		void SetCursor(Material material, Vec2 pivot);
		void SetSelectedObject(UIObject* object);

	private:
		void HandleButtonLeftPressed();

		MessageBus* m_MessageBus;

		UIObject* m_SelectedObject;

		Cursor m_Cursor;
		std::vector<std::shared_ptr<UIObject>> m_UIObjects;
		std::map<std::string, std::shared_ptr<UIWindow>> m_UIWindows;
		std::vector<std::shared_ptr<UIText>> m_UITexts;
		std::vector<std::shared_ptr<UIButton>> m_UIButtons;
		std::vector<std::shared_ptr<UIImage>> m_UIImages;
		std::vector<std::shared_ptr<UIDropDown>> m_UIDropDowns;
	};
}