#pragma once
#include "Texture.h"

namespace Deft 
{
	//Structure for materials (shader and texture pair).
	struct Material
	{
		unsigned int materialID;		//ID number of the material (assign when material is created).
		unsigned int shaderID;			//ID number of the shader program that will be used to draw this texture.
		Texture texture;				//Texture.
	};
}