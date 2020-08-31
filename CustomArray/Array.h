#pragma once

#include <cassert>
#include <iostream>

#ifndef CUSTOMARRAY
#define CUSTOMARRAY

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

	int size() const { return m_size; }

	T& operator[](int index);

	friend std::ostream& operator<<(std::ostream &out, const Array<T> &array)
	{
		for (int i{ 0 }; i < array.size(); ++i)
		{
			out << array.m_array[i] << " ";
		}

		return out;
	}
};

#endif