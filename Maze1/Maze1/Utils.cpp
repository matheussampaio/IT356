#include "Utils.h"

bool Utils::isPointEqual(float x1, float y1, float x2, float y2)
{
    return (x1 == x2 && y1 == y2);
}

bool Utils::isPointInSquare(float x, float y, float squareX1, float squareY1, float squareX2, float squareY2)
{
    return (squareX1 <= x && x <= squareX2 && squareY1 <= y && y <= squareY2);
}

int Utils::bitsetToInt(std::bitset<4> bitset)
{
    int valueInInt = 0;

    for (int i = 0; i < 4; i++)
    {
        if (bitset[3 - i])
        {
            valueInInt += pow(2, 3 - i);
        }
    }

    return valueInInt;
}

int Utils::randomInt(int minNumber, int maxNumber)
{
    return rand() % (maxNumber + 1 - minNumber) + minNumber;
}