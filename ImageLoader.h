#pragma once

#include "Texture.h"
#include <string>

namespace Deft
{
	class ImageLoader
	{
	public:
		static Texture LoadPNG(std::string filePath);
	};
}
