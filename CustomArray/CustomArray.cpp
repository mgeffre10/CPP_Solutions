// CustomArray.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Array.h"

#include <iostream>

int main()
{
    /*Array<int> arr{5};

    std::cout << arr << '\n';
    std::cout << arr[0] << '\n';

    arr[0] = 2;
    arr[1] = 3;

    //std::cout << arr[0] << " " << arr[1] << '\n';

    arr.replace(1, 6);

    std::cout << arr << '\n';

    std::cout << arr.subArr(0, 2) << '\n';*/

    Array<int> arr2{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    std::cout << arr2 << '\n';

    return 0;
}
