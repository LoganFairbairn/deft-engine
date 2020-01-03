#pragma once
#include <GL/glew.h>
#include "Vector.h"
#include "Color.h"

namespace Deft
{
	//This is the default vertex struct which contains position, color, and uv data for a vertice.
	struct Vertex
	{
		GLVec3 position;		//x, y, z (depth)
		ColorRGBA8 color;
		GLVec2 uv;

		Vertex() {}

		Vertex(GLVec3 p, ColorRGBA8 c, GLVec2 uvCoords)
		{
			position = p;
			color = c;
			uv = uvCoords;
		}

		//Having functions inside of a struct doesn't increase the memory the struct uses.
		void SetPosition(GLfloat x, GLfloat y)
		{
			position.x = x;
			position.y = y;
		}

		void SetColor(ColorRGBA8 Color)
		{
			color.r = Color.r;
			color.g = Color.g;
			color.b = Color.b;
			color.a = Color.a;
		}

		void SetColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
		{
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}

		void SetUV(GLfloat u, GLfloat v)
		{
			uv.x = u;
			uv.y = v;
		}

		void SetDepth(GLfloat depth)
		{
			position.z = depth;
		}
	};

	//This vertex contains position and color data but no normal data.
	struct PrimitiveVertex
	{
		//This needs to be a glm vec2 because it is guarenteed to be 32 bits even on different compilers.
		GLVec3 position;		//x, y, z (depth)
		ColorRGBA8 color;

		PrimitiveVertex()
		{
			//Empty.
		}

		PrimitiveVertex(const GLVec2& p, const ColorRGBA8& c)
		{
			position.x = p.x;
			position.y = p.y;
			color = c;
		}

		void SetPosition(GLfloat x, GLfloat y)
		{
			position.x = x;
			position.y = y;
		}

		void SetColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
		{
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}

		void SetDepth(float depth)
		{
			position.z = depth;
		}
	};
}