#include "UIImage.h"

namespace Deft
{
	void UIImage::Update(Vec2 mousePosition)
	{

	}

	void UIImage::Draw(Renderer* renderer)
	{
		renderer->DrawSprite(m_Image, m_Position);
	}
}