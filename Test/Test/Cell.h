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

    /* Base Coordinates of the Cells */
    int mX, mY, mIndex;

    /* Offsets */
    int mLeftOffset, mTopOffset;

    /* Screen Ratio */
    int mRatioWidth, mRatioHeigth;

    VertexAttribs mVertex[4];

public:

    Cell(int index, int x, int y, std::bitset<4> walls, int ratioHeigth, int ratioWidth);

    bool update(int x1, int y1, int x2, int y2);

    void removeWall(int x1, int y1, int x2, int y2);

    void appendVertexIndex(std::vector<GLuint> *vertexIndex);

    void appendVertexData(std::vector<VertexAttribs> *vertexData);

    int getX() { return mX; };
    int getY() { return mY; };
    int getWallsInInt() { return Utils::bitsetToInt(mWalls); };

};

#endif