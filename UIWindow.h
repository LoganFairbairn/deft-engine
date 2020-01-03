#pragma once
#include "UIObject.h"
#include "UISystem.h"
#include "MathFunctions.h"

namespace Deft
{
	class UISystem;
	class UIObject;

	class UIWindow : public UIObject
	{
	public:
		UIWindow(UISystem* system, Vec2 position, Vec2 size);
		~UIWindow() {};

		void Update(Vec2 mousePosition);
		void Draw(Renderer* renderer);
		void AddObject(UIObject* object);

		//Setters.
		void SetDepth(float depth);
		void SetPreviousPosition(Vec2 previousPosition) { m_PreviousPosition = previousPosition; }
		void SetName(const std::string& name) { m_Name = name; }
		void SetBarColor(ColorRGBA8 color) { m_BarColor = color; }
		void SetMouseGrabOffSet(Vec2 offSet) { m_MouseGrabOffSet = offSet; }
		void SetHeld(bool held) { m_Held = held; }
		void ShowWindow();
		void HideWindow();

		//Getters.
		std::string GetName() { return m_Name; }
		Vec2 GetMouseGrabOffSet() { return m_MouseGrabOffSet; }
		bool GetHeld() { return m_Held; }

	private:
		void KeepWindowOnScreen(unsigned int cameraWidth, unsigned int cameraHeight);
		void AddCloseButton(UISystem* system);
		void MoveWindowObjects();

		std::string m_Name;
		ColorRGBA8 m_BarColor;
		float m_BarHeight;
		float m_BarPadding;
		SpriteFont* m_BarFont;

		Vec2 m_PreviousPosition;

		float m_Padding;

		Vec2 m_MouseGrabOffSet;
		bool m_Held;

		bool m_WasHidden;

		UIButton* m_CloseButton;

		std::vector<UIObject*> m_UIObjects;
	};
}