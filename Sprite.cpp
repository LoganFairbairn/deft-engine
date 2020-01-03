#include "Sprite.h"

namespace Deft
{
	Sprite::Sprite()
	{
		//Resize the vertices.
		m_Vertices.resize(4);
	}

	//Creates a sprite from a texture (gives the sprite the default shader).
	Sprite::Sprite(Material material, Vec2 position, Vec2 size, float rotation, Vec2 originPoint, ColorRGBA8 color, float depth, Vec4 uvRectangle)
	{
		//Resize the vertices (4 for a square).
		m_Vertices.resize(4);

		//Set sprite variables.
		SetMaterial(material);
		SetTransform(position, Vec2(size.x, size.y), originPoint);
		SetUVRectangle(uvRectangle);
		SetColor(color);
		SetDepth(depth);

		if (rotation != 0)
		{
			SetRotation(rotation);
		}
	}

	//Updates the vertices in the sprite with the given position, origin point, and size.
	void Sprite::SetTransform(Vec2 position, Vec2 size, Vec2 originPoint)
	{
		m_Vertices[0].SetPosition(position.x + size.x - originPoint.x, position.y + size.y - originPoint.y);
		m_Vertices[1].SetPosition(position.x - originPoint.x, position.y + size.y - originPoint.y);
		m_Vertices[2].SetPosition(position.x - originPoint.x, position.y - originPoint.y);
		m_Vertices[3].SetPosition(position.x + size.x - originPoint.x, position.y - originPoint.y);
	}

	//Updates the vertices uv values with the given uv rectangle.
	void Sprite::SetUVRectangle(Vec4 uvRectangle)
	{
		m_Vertices[0].SetUV(uvRectangle.x + uvRectangle.z, uvRectangle.y + uvRectangle.w);
		m_Vertices[1].SetUV(uvRectangle.x, uvRectangle.y + uvRectangle.w);
		m_Vertices[2].SetUV(uvRectangle.x, uvRectangle.y);
		m_Vertices[3].SetUV(uvRectangle.x + uvRectangle.z, uvRectangle.y);
	}

	//Sets the color for all the vertices in the sprite.
	void Sprite::SetColor(ColorRGBA8 color)
	{
		for (int i = 0; i < 4; i++)
		{
			m_Vertices[i].SetColor(color);
		}
	}

	//Rotates all of the vertices in the sprite by a specified rotation.
	void Sprite::SetRotation(float angle)
	{
		for (int i = 0; i < 4; i++)
		{
			GLfloat x = m_Vertices[i].position.x;
			GLfloat y = m_Vertices[i].position.y;

			Vec2 verticePosition = RotatePoint(x, y, angle);
			m_Vertices[i].SetPosition(verticePosition.x, verticePosition.y);
		}
	}

	//Sets the depth for all of the vertices.
	void Sprite::SetDepth(float depth)
	{
		for (int i = 0; i < m_Vertices.size(); i++)
		{
			m_Vertices[i].SetDepth(depth);
		}
	}

	//Returns the depth of the sprite (all vertices in a sprite should have the same z value).
	float Sprite::GetDepth()
	{
		return m_Vertices[0].position.z;
	}

	//Rotates a 2D point around the select position.
	Vec2 Sprite::RotatePoint(float x, float y, float angle)
	{
		Vec2 newVector;

		newVector.x = x * cos(angle) - y * sin(angle);
		newVector.y = x * sin(angle) + y * cos(angle);
		return newVector;
	}
}