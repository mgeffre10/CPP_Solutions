// TextAdventure.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm> // count_if
#include <ctime> // time
#include <iostream>
#include <string>
#include <random> // mt19937
#include <vector>

namespace Random
{
    // Initialize our mersenne twister with a random seed based on the clock (once at system startup)
    std::mt19937 mersenne{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };
}

// Intro Function
void intro();

// Game Loop
void gameLoop();

// Outro Function
void outro();

int main()
{
    // Intro
    intro();
    // Game Loop
    gameLoop();
    // Outro
    outro();
}