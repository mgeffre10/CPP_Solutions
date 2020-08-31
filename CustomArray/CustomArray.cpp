// CustomArray.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Array.h"

#include <iostream>

int main()
{
    Array<int> arr{5};

    std::cout << arr << '\n';
    std::cout << arr[0] << '\n';
    return 0;
}
