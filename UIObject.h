#pragma once
#include "Vector.h"
#include "Vertex.h"
#include "Renderer.h"
#include "UIGroup.h"

namespace Deft
{
	//Every UI object must inherit from this class.
	class UIObject
	{
	public:
		UIObject() {};
		~UIObject() {};

		virtual void Update(Vec2 mousePosition) = 0;		//All UI objects will display a description if the mouse is over their bounding box.
		virtual void Draw(Renderer* renderer) = 0;			//UI objects require a pointer to the renderer to draw.

		//Setters.
		void SetAutoDraw(bool autoDraw) { m_AutoDraw = autoDraw; }
		void SetSelected(bool selected) { m_Selected = selected; }
		void SetID(unsigned int id) { m_ID = id; }
		void SetGroup(UIGroup group) { m_Group = group; }
		void SetVisible(bool visible) { m_Visible = visible; }
		void SetMouseIsOver(bool mouseIsOver) { m_MouseIsOver = mouseIsOver; }
		void SetDepth(float depth) { m_Depth = depth; }
		void SetPosition(Vec2 position) { m_Position = position; }
		void SetOriginalPosition(Vec2 originalPosition) { m_OriginalPosition = originalPosition; }
		void SetScale(Vec2 scale) { m_Scale = scale; }
		void SetSize(Vec2 size) { m_Size = size; }
		void SetPivot(Vec2 pivot) { m_Pivot = pivot; }
		void SetColor(ColorRGBA8 color) { m_Color = color; }
		void SetDescription(const std::string& description) { m_Description = description; }

		//Getters.
		bool GetAutoDraw() { return m_AutoDraw; }
		bool GetSelected() { return m_Selected; }
		unsigned int GetID() { return m_ID; }
		UIGroup GetGroup() { return m_Group; }
		bool GetVisible() { return m_Visible; }
		bool GetMouseIsOver() { return m_MouseIsOver; }
		float GetDepth() { return m_Depth; }
		float GetOriginalDepth() { return m_OriginalDepth; }
		Vec2 GetPosition() { return m_Position; }
		Vec2 GetOriginalPosition() { return m_OriginalPosition; }
		Vec2 GetScale() { return m_Scale; }
		Vec2 GetSize() { return m_Size; }
		Vec2 GetPivot() { return m_Pivot; }
		ColorRGBA8 GetColor() { return m_Color; }
		std::string GetDescription() { return m_Description; }

	protected:
		bool m_AutoDraw = true;
		bool m_Selected = false;
		unsigned int m_ID;										//The ID for this UI object.
		UIGroup m_Group = UIGroup::UI_GROUP_MAIN;				//The group this UI object is a part of.
		bool m_Visible = true;
		bool m_MouseIsOver = false;								//If the mouse is over this objects bounding box.
		float m_Depth = -0.5f;									//The default depth for UI objects is -0.5f.
		float m_OriginalDepth = -0.5f;
		Vec2 m_Position = Vec2(0.0f, 0.0f);
		Vec2 m_OriginalPosition = Vec2(0.0f, 0.0f);
		Vec2 m_Scale = Vec2(1.0f, 1.0f);
		Vec2 m_Size = Vec2(0.0f, 0.0f);							//Width, height of the UI object.
		Vec2 m_Pivot = Vec2(0.0f, 0.0f);
		ColorRGBA8 m_Color = ColorRGBA8(255, 255, 255, 255);
		std::string m_Description = "";
	};
}