#ifndef UTILS_H
#define UTILS_H

#include <bitset>
#include <cmath>

class Utils
{

public:
    static bool isPointEqual(float x1, float y1, float x2, float y2);
    static bool isPointInSquare(float x, float y, float squareX1, float squareY1, float squareX2, float squareY2);
    static int bitsetToInt(std::bitset<4> bitset);
    static int randomInt(int minNumber, int maxNumber);

};

#endif