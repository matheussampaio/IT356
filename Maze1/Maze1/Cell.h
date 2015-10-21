#ifndef CELL_H
#define CELL_H

#include <bitset>
#include <iostream>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GL/gl.h>

#include "VertexAttribs.h"
#include "Utils.h"

#include <SFML/Graphics.hpp>

class Cell
{
    /* Bits representing the walls*/
    std::bitset<4> mWalls;
    bool mWallsUnblocked[4];

    /* Base Coordinates of the Cells */
    float mX, mY;
    int mLeftTopIndex;

    MyVertexAttribs mVertex[4];

    bool wallIsEqual(int begin, int end, float x1, float y1, float x2, float y2);


public:

    Cell(int index, float x, float y, std::bitset<4> walls);

    bool update(float x1, float y1, float x2, float y2);

    void removeWall(float x1, float y1, float x2, float y2);

    void appendVertexIndex(std::vector<GLuint> *vertexIndex);
    void appendVertexData(std::vector<MyVertexAttribs> *vertexData, float ratioWidth, float ratioHeigth, float padding);

    void blockWall(int wall);

    float getX() { return mX; };
    float getY() { return mY; };

    int getWallsInInt() { return Utils::bitsetToInt(mWalls); };

};

#endif