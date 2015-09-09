#include "Utils.h"

bool Utils::isPointEqual(int x1, int y1, int x2, int y2)
{
    return (x1 == x2 && y1 == y2);
}

bool Utils::isPointInSquare(int x, int y, int squareX1, int squareY1, int squareX2, int squareY2)
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