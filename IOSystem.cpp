#include "IOSystem.h"
#include <fstream>

namespace Deft
{
	//Reads and stores a file inside a vector or characters.
	bool IOSystem::ReadFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer)
	{
		//Open the file.
		std::ifstream file(filePath, std::ios::binary);

		//Check for errors when opening the file. 
		if (file.fail())
		{
			perror(filePath.c_str());
			return false;
		}

		//Get the size of the file by seeking to the end of the file, and then counting the number of bytes passed.
		file.seekg(0, std::ios::end);
		int fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		//Reduce the file size by any header bytes that might be present (this shouldn't matter for most files).
		fileSize -= file.tellg();

		//Resize the buffer we're using to the size of the file that's being read.
		buffer.resize(fileSize);

		//Read the whole file as characters.
		file.read((char *)&(buffer[0]), fileSize);

		//Close the file after it's finished being read.
		file.close();

		//Returns true because the file has successfully been read.
		return true;
	}
}