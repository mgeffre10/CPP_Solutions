// CustomArray.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Array.h"

#include <iostream>

int main()
{
    Array<int> arr{ 1, 2, 4, 5, 6 };

    std::cout << arr << '\n';

    arr.insertAt(2, 3);
    arr.prepend(0);
    arr.append(7);

    std::cout << arr << '\n';

    int removeSelectedIndex = arr.removeAt(2);
    int removeFront = arr.pop_front();
    int removeBack = arr.pop_back();

    std::cout << "Removed element " << removeSelectedIndex << " at index 2.\n";
    std::cout << "Removed element " << removeFront << " from the front.\n";
    std::cout << "Removed element " << removeBack << " from the back.\n";

    std::cout << arr << '\n';

    return 0;
}
