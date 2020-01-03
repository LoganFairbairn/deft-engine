#pragma once
#include "GL/glew.h"

namespace Deft
{
	//Handy structure for textures.
	struct Texture
	{
		GLuint id;
		unsigned int width;
		unsigned int height;
	};
}