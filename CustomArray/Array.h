//#pragma once
#ifndef CUSTOMARRAY
#define CUSTOMARRAY

#include <cassert>
#include <iostream>
#include <initializer_list>

template <typename T>
class Array {
	T *m_array;
	int m_size;

public:

	Array(int size)
	{
		std::cout << "In Array constructor.\n";

		assert(size > 0);
		m_array = new T[size]{};
		m_size = size;
	}

	Array(std::initializer_list<T> list) : Array(static_cast<int>(list.size()))
	{
		assert(list.size() > 0);

		int arrayIndex{ 0 };
		for (T element : list)
		{
			m_array[arrayIndex] = element;
			++arrayIndex;
		}
	}

	T &operator[](int index)
	{
		assert(isInBounds(index));
		return m_array[index];
	}

	friend std::ostream &operator<<(std::ostream &out, const Array<T> &array)
	{
		for (int i{ 0 }; i < array.size(); ++i)
		{
			out << array.m_array[i] << " ";
		}

		return out;
	}

	int size() const { return m_size; }

	bool isInBounds(const int index)
	{
		return index >= 0 && index < m_size;
	}

	T& getAt(int index)
	{
		assert(isInBounds(index));

		return m_array[index];
	}

	void replace(const int index, const T &element)
	{
		assert(isInBounds(index));
		m_array[index] = element;
	}

	void insertAt(const int index, const T &element)
	{
		assert(index >= 0 && index <= m_size);

		T *newArr{ new T[m_size + 1] };

		for (int before{ 0 }; before < index; ++before)
		{
			newArr[before] = m_array[before];
		}

		newArr[index] = element;

		for (int after{ index }; after < m_size; ++after)
		{
			newArr[after + 1] = m_array[after];
		}

		delete[] m_array;
		m_array = newArr;
		++m_size;
	}

	void prepend(const T &element)
	{
		insertAt(0, element);
	}

	void append(const T &element)
	{
		insertAt(m_size, element);
	}

	T removeAt(const int index)
	{
		T *newArr{ new T[m_size - 1] };

		int newArrIndex{ 0 };

		for (int i{ 0 }; i < m_size; ++i)
		{
			if (i == index) continue;

			newArr[newArrIndex] = m_array[i];
			++newArrIndex;
		}

		T returnValue = m_array[index];

		delete[] m_array;
		m_array = newArr;
		--m_size;

		return returnValue;
	}

	T pop_front()
	{
		return removeAt(0);
	}

	T pop_back()
	{
		return removeAt(m_size - 1);
	}

	Array<T> subArr(const int startIndex, const int endIndex = m_size)
	{
		assert(endIndex > startIndex && startIndex != endIndex);

		int newArrSize{ endIndex - startIndex };
		Array<T> newArr{ newArrSize };
		int newArrIndex{ 0 };

		for (int i{ startIndex }; i < endIndex; ++i)
		{
			newArr[newArrIndex] = m_array[i];
			++newArrIndex;
		}

		return newArr;
	}
};

#endif