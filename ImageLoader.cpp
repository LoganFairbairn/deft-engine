#include "ImageLoader.h"
#include "picoPNG.h"
#include "IOSystem.h"
#include "FatalErrors.h"

namespace Deft
{
	Texture ImageLoader::LoadPNG(std::string filePath)
	{
		Texture texture = {};

		std::vector<unsigned char> in;
		std::vector<unsigned char> out;

		unsigned long width, height;

		if (IOSystem::ReadFileToBuffer(filePath, in) == false)
		{
			FatalError("Failed to load PNG file to buffer.");
		}

		//Use picoPNG to check for errors.
		int errorCode = decodePNG(out, width, height, &(in[0]), in.size());
		if (errorCode != 0)
		{
			FatalError("decodePNG failed: " + std::to_string(errorCode));
		}

		//Generates 1 texture name, and stores it in a texture ID.
		glGenTextures(1, &(texture.id));

		//Binds a named texture to a texturing target.
		glBindTexture(GL_TEXTURE_2D, texture.id);

		//Specifies a two-dimensional texture image (allows elements of an image array to be read by shaders).
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

		//Set texture parameters (wrapping, mip mapping).
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		//Generates a mipmap.
		glGenerateMipmap(GL_TEXTURE_2D);

		//Unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);

		//Store the texture width and height (for convenience).
		texture.width = width;
		texture.height = height;

		return texture;
	}
}