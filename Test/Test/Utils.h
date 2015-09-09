#ifndef UTILS_H
#define UTILS_H

#include <bitset>
#include <cmath>

class Utils
{

public:
    static bool isPointEqual(int x1, int y1, int x2, int y2);
    static bool isPointInSquare(int x, int y, int squareX1, int squareY1, int squareX2, int squareY2);
    static int bitsetToInt(std::bitset<4> bitset);
    static int randomInt(int minNumber, int maxNumber);

};

#endif