#pragma once
#include "UIObject.h"

namespace Deft
{
	class UIImage : public UIObject
	{
	public:
		UIImage() {};
		~UIImage() {};

		void Update(Vec2 mousePosition);
		void Draw(Renderer* renderer);

		//Setters.
		void SetImage(Material material) { m_Image = material; }

		//Getters.
		Material GetImage() { return m_Image; }

	private:
		Material m_Image;
	};
}