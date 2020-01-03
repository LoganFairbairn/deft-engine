#include "MathFunctions.h"

namespace Deft
{
	//Returns true if the given position is within the box boundaries.
	bool IsInBox(Vec2 position, Vec2 boxTopLeft, Vec2 boxBottomRight)
	{
		if (position.x >= boxTopLeft.x && position.x <= boxBottomRight.x)
		{
			if (position.y <= boxTopLeft.y && position.y >= boxBottomRight.y)
			{
				return true;
			}
		}

		return false;
	}
}