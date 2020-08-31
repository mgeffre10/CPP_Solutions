#pragma once

#include <iostream>

#ifndef CUSTOMARRAY
#define CUSTOMARRAY

template <typename T>
class Array {
	T *m_array{};
	int m_size{};

public:
	Array(int size=0) : m_size{ size }
	{
		std::cout << "In Array constructor.\n";
	}
};

#endif