#include "HashString.h"


namespace Deft
{
	HashString::HashString()
	{
	}

	HashString::~HashString()
	{
	}

	//Adds an element to the hash table.
	void HashString::AddElement(const std::string& key)
	{
		//Use a hash algorithm to calculate the index for this key value.
		unsigned int index = CalculateIndex(key);

		//Store the element in a linked list (this is to avoid collisions for index storage).
		m_HashTable[index].CreateNode(key);

		std::cout << "\nAdded " + key + " to linked list (" + std::to_string(index) + ")";
		//std::cout << "\nNumber of nodes in this LinkedList: " + std::to_string(m_HashTable[index].CountElements());
	}

	//Gets an element from the hash table.
	std::string HashString::GetElement(const std::string& key)
	{
		unsigned int index = CalculateIndex(key);

		m_HashTable[index].OutputList();

		if (m_HashTable[index].GetFirstElement() != key)
		{
			//Cycle through the linked list, look for the value that matches the key.
			unsigned int i = m_HashTable[index].FindElement(key);

			std::string element = m_HashTable[index].GetElementAt(i);
			return element;
		}

		else
		{
			return m_HashTable[index].GetFirstElement();
		}
	}

	//Deletes an element from the hash table.
	void HashString::DeleteElement()
	{

	}

	//Outputs all of the elements in the hash table.
	void HashString::OutputTable()
	{

	}

	//Calculates the index.
	const unsigned int HashString::CalculateIndex(const std::string& key)
	{
		unsigned int index = 0;

		for (int i = 0; i < key.length(); i++)
		{
			index += (int)key[i];
		}

		index = index % TABLE_SIZE;

		return index;
	}
}
