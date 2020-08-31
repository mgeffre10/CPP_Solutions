#include "Array.h"

template<typename T>
T& Array<T>::operator[](int index)
{
	assert(index >= 0 && index < m_size);

	return m_array[index];
}