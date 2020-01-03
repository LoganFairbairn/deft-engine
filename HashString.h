#pragma once
#include <vector>
#include <string>
#include <iostream>			//Used for testing remove later.
#include "LinkedList.h"		//Used for handling collisions in the Hash Table.

namespace Deft
{
	const static int TABLE_SIZE = 20;

	//TODO: Check for collisions.
	class HashString
	{
	public:
		HashString();
		~HashString();

		void AddElement(const std::string& key);
		std::string GetElement(const std::string& key);
		void DeleteElement();
		void OutputTable();
		const unsigned int CalculateIndex(const std::string& key);

	private:
		LinkedList<std::string> m_HashTable[TABLE_SIZE];
	};
}

