#pragma once
#include <GL\glew.h>
#include <cmath>

namespace Deft
{
	//These are some handy structures for writing with vectors.
	struct IVec2
	{
		IVec2() : x(0), y(0)
		{

		}

		IVec2(int X, int Y) : x(X), y(Y)
		{

		}

		//Gets the magnitude of the vector.
		float GetMagnitude()
		{
			return sqrt(pow(x, 2) + pow(y, 2));
		}

		int x;
		int y;
	};

	//These are some handy structures for writing with vectors.
	struct Vec2
	{
		Vec2() : x(0), y(0)
		{

		}

		Vec2(float X, float Y) : x(X), y(Y)
		{

		}

		//Gets the magnitude of the vector.
		float GetMagnitude()
		{
			return sqrt( pow(x, 2) + pow(y, 2));
		}

		float x;
		float y;
	};

	struct Vec3
	{
		Vec3() : x(0), y(0), z(0)
		{

		}

		Vec3(float X, float Y, float Z) : x(X), y(Y), z(Z)
		{

		}

		//Gets the magnitude of the vector.
		float GetMagnitude()
		{
			return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
		}

		float x;
		float y;
		float z;
	};

	struct Vec4
	{
		Vec4() : x(0), y(0), z(0)
		{

		}

		Vec4(float X, float Y, float Z, float W) : x(X), y(Y), z(Z), w(W)
		{

		}

		float x;
		float y;
		float z;
		float w;
	};

	//These are some handy structures for writing with vectors while dealing with graphics language.
	//Using GLfloat guarentees the size of the float will be 32 bits accross compilers and platforms.
	struct GLVec2
	{
		GLVec2() : x(0), y(0)
		{

		}

		GLVec2(GLfloat X, GLfloat Y) : x(X), y(Y)
		{

		}

		GLfloat x;
		GLfloat y;
	};

	struct GLVec3
	{
		GLVec3() : x(0), y(0), z(0)
		{

		}

		GLVec3(GLfloat X, GLfloat Y, GLfloat Z) : x(X), y(Y), z(Z)
		{

		}

		GLfloat x;
		GLfloat y;
		GLfloat z;
	};

	struct GLVec4
	{
		GLVec4() : x(0), y(0), z(0)
		{

		}

		GLVec4(GLfloat X, GLfloat Y, GLfloat Z, GLfloat W) : x(X), y(Y), z(Z), w(W)
		{

		}

		GLfloat x;
		GLfloat y;
		GLfloat z;
		GLfloat w;
	};
}