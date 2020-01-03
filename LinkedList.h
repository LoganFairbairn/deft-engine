#pragma once
#include <iostream>
#include <string>

#include "FatalErrors.h"

namespace Deft
{
	template<class T>
	class LinkedList
	{
	public:
		LinkedList();
		~LinkedList();

		void InsertStart(T value);
		void InsertPosition(unsigned int index, T value);
		void CreateNode(T value);

		void DeleteFirst();
		void DeleteLast();
		void DeletePosition(unsigned int index);

		unsigned int CountElements();
		T GetFirstElement();
		T GetElementAt(unsigned int index);
		unsigned int FindElement(T data);

		void OutputList();

	private:
		struct Node
		{
			T data;
			Node* next;
		};

		Node* m_Head;		//Start of the linked list.
		Node* m_Tail;		//End of the linked list.
	};

	template<class T>
	LinkedList<T>::LinkedList()
	{
		m_Head = nullptr;
		m_Tail = nullptr;
	}

	template<class T>
	LinkedList<T>::~LinkedList()
	{
	}

	//Inserts a node at the start of the linked list.
	template<class T>
	void LinkedList<T>::InsertStart(T value)
	{
		Node* node = new Node;
		node->x = value;
		node->next = m_Head;
		m_Head = node;
	}

	//Inserts a node at the end of the linked list.
	template<class T>
	void LinkedList<T>::InsertPosition(unsigned int index, T value)
	{
		Node* previous = new Node;
		Node* current = new Node;
		Node* = new Node;

		current = m_Head;
		for (int i = 1; i < position; i++)
		{
			previous = current;
			current = current->next;
		}

		node->data = value;
		previous->next = node;
		node->next = current;
	}

	//Creates a node at the end of the linked list.
	template<class T>
	void LinkedList<T>::CreateNode(T value)
	{
		Node* node = new Node;

		node->data = value;
		node->next = nullptr;

		if (m_Head == nullptr)
		{
			m_Head = node;
			m_Tail = node;
			node = nullptr;
		}

		else
		{
			m_Tail->next = node;
			m_Tail = node;
		}
	}

	//Deletes the first node in a linked list.
	template<class T>
	void LinkedList<T>::DeleteFirst()
	{
		Node* node = new Node;
		node = m_Head;
		m_Head = m_Head->next;
		delete node;
	}

	//Deletes the last node in a linked list.
	template<class T>
	void LinkedList<T>::DeleteLast()
	{
		Node* current = new Node;
		Node* previous = new Node;
		current = m_Head;

		while (current->next != nullptr)
		{
			previous = current;
			current = current->next;
		}

		m_Tail = previous;
		previous->next = nullptr;
		delete current;
	}

	//Deletes a node at a specified position.
	template<class T>
	void LinkedList<T>::DeletePosition(unsigned int index)
	{
		Node* current = new Node;
		Node* previous = new Node;
		current = m_Head;

		for (int i = 1; i < position; i++)
		{
			previous = current;
			current = current->next;
		}

		previous->next = current->next;
	}

	//Counts the number of elements in the list.
	template<class T>
	unsigned int LinkedList<T>::CountElements()
	{
		Node* current = new Node;
		current = m_Head;

		unsigned int numberOfElements = 0;

		if (m_Head != nullptr)
		{
			numberOfElements = 1;
		}

		while (current->next != nullptr)
		{
			if (current->next == nullptr)
			{
				return numberOfElements;
			}

			else
			{
				current = current->next;
				numberOfElements++;
			}
		}

		return numberOfElements;
	}

	//Gets the first element of the linked list.
	template<class T>
	T LinkedList<T>::GetFirstElement()
	{
		if (m_Head != nullptr)
		{
			return m_Head->data;
		}
		
		else
		{
			FatalError("\nLinkedList head is null.");
		}
	}

	//Gets the element at the specified index in the linked list.
	template<class T>
	T LinkedList<T>::GetElementAt(unsigned int index)
	{
		Node* current = new Node;
		current = m_Head;

		for (int i = 0; i < index + 1; i++)
		{
			if (i == index)
			{
				return current->data;
			}

			else
			{
				if (current->next != nullptr)
				{
					current = current->next;
				}

				else
				{
					FatalError("\n LinkedList hit a null pointer.");
				}
			}
		}

		return m_Head->data;
	}

	//Cycles through all elements in the list looking for a specified piece of data. Returns the index of the data.
	template<class T>
	unsigned int LinkedList<T>::FindElement(T data)
	{
		std::cout << "\nLooking for data: " + data;
		Node* current = new Node;
		current = m_Head;

		unsigned int index = 0;
		while (current->next != nullptr)
		{
			if (current->data == data)
			{
				std::cout << "\nFound data: " + data + " at linked list index " + std::to_string(index);
				return index;
			}

			current = current->next;
			index++;

			if (current->next == nullptr)
			{
				if (current->data == data)
				{
					std::cout << "\nFound data: " + data + " at linked list index " + std::to_string(index);
					return index;
				}
			}
		}

		FatalError("\nError: Data not found in Linked List.");
	}

	//Outputs the linked list to the screen (helpful for debugging).
	template<class T>
	void LinkedList<T>::OutputList()
	{
		Node* node = new Node;
		node = m_Head;

		int i = 0;
		while (node != nullptr)
		{
			std::cout << "\nIndex: " + std::to_string(i) + " = " + node->data;
			node = node->next;
			i++;
		}
	}
}

