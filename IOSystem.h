#pragma once
#include <vector>

namespace Deft
{
	//Handles all reading and writing to files.
	class IOSystem
	{
	public:
		static bool ReadFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);
	};
}
