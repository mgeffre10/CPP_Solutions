#pragma once
#ifndef TEXTADVENTURE_RANDOM
#define TEXTADVENTURE_RANDOM

#include <ctime> // time
#include <random> // mt19937

namespace Random
{
    // Initialize our mersenne twister with a random seed based on the clock (once at system startup)
    static std::mt19937 mersenne{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };

    static inline int getRandomNumberInRange(int min, int max)
    {
        std::uniform_int_distribution randomNum{ min, max };

        return randomNum(mersenne);
    }
};

#endif