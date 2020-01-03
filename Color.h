#pragma once
#include "Vector.h"

namespace Deft
{
	//8 Bit channel with RGBA color values.
	struct ColorRGBA8
	{
		//Default constructor sets RGBA color values to 0.
		ColorRGBA8() : r(0), g(0), b(0), a(0)
		{

		}

		ColorRGBA8(GLubyte R, GLubyte G, GLubyte B, GLubyte A) : r(R), g(G), b(B), a(A)
		{

		}

		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	};
}