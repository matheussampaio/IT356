#include "Cell.h"
#include "Utils.h"

Cell::Cell(int index, float x, float y, std::bitset<4> walls)
{
    mX = x;
    mY = y;
    mWalls = walls;
    mLeftTopIndex = index;
    
    // left - top : x, y
    mVertex[0].setXYZW(mX , mY, 0, 1);

    // right - top : x + 1, y
    mVertex[1].setXYZW((mX + 1), mY, 0, 1);

    // right - bottom : x + 1, y + 1
    mVertex[2].setXYZW((mX + 1), (mY + 1), 0, 1);

    // left - bottom : x, y + 1
    mVertex[3].setXYZW(mX, (mY + 1), 0, 1);
}

bool Cell::update(float x1, float y1, float x2, float y2)
{
    /* check if any vertex are inside of the square */
    bool leftTopVertexInside = Utils::isPointInSquare(mX, mY, x1, y1, x2, y2);
    bool rightTopVertexInside = Utils::isPointInSquare((mX + 1), mY, x1, y1, x2, y2);
    bool rightBottomVertexInside = Utils::isPointInSquare((mX + 1), (mY + 1), x1, y1, x2, y2);
    bool leftBottomVertexInside = Utils::isPointInSquare(mX, (mY + 1), x1, y1, x2, y2);

    /* if any vertex are inside of the square */
    if (leftTopVertexInside || leftBottomVertexInside || rightTopVertexInside || rightBottomVertexInside)
    {
        /* if at least one vertex of the wall is inside of the square, remove THAT wall, otherwise, ADD that wall. */
        if (mWallsUnblocked[3]) {
            mWalls[3] = !(leftTopVertexInside || leftBottomVertexInside);
        }

        if (mWallsUnblocked[2]) {
            mWalls[2] = !(rightTopVertexInside || leftTopVertexInside);
        }
        
        if (mWallsUnblocked[1]) {

            mWalls[1] = !(rightTopVertexInside || rightBottomVertexInside);
        }

        if (mWallsUnblocked[0]) {
            mWalls[0] = !(leftBottomVertexInside || rightBottomVertexInside);
        }

        return true;
    }

    return false;
}

void Cell::removeWall(float x1, float y1, float x2, float y2)
{
    /* TOP */
    if (wallIsEqual(0, 1, x1, y1, x2, y2))
    {
        printf("TOP wall removed: %.2f,%.2f -> %.2f,%.2f\n", x1, y1, x2, y2);
        mWalls[2] = 0;

    /* RIGHT */
    }
    else if (wallIsEqual(1, 2, x1, y1, x2, y2))
    {
        printf("RIGHT wall removed: %.2f,%.2f -> %.2f,%.2f\n", x1, y1, x2, y2);
        mWalls[1] = 0;

    /* BOTTOM */
    }
    else if (wallIsEqual(2, 3, x1, y1, x2, y2))
    {
        printf("BOTTOM wall removed: %.2f,%.2f -> %.2f,%.2f\n", x1, y1, x2, y2);
        mWalls[0] = 0;

    /* LEFT */
    }
    else if (wallIsEqual(3, 0, x1, y1, x2, y2))
    {
        printf("LEFT wall removed: %.2f,%.2f -> %.2f,%.2f\n", x1, y1, x2, y2);
        mWalls[3] = 0;
    }
}

bool Cell::wallIsEqual(int begin, int end, float x1, float y1, float x2, float y2)
{
    GLfloat vX1, vY1, vX2, vY2;

    vX1 = mVertex[begin].getX();
    vY1 = mVertex[begin].getY();

    vX2 = mVertex[end].getX();
    vY2 = mVertex[end].getY();

    bool wallIn = Utils::isPointEqual(x1, y1, vX1, vY1) && Utils::isPointEqual(x2, y2, vX2, vY2);
    bool wallBack = Utils::isPointEqual(x1, y1, vX2, vY2) && Utils::isPointEqual(x2, y2, vX1, vY1);

    return wallIn || wallBack;
}

void Cell::appendVertexIndex(std::vector<GLuint> *vertexIndex) {

    int firstIndex = mLeftTopIndex;

    /*
    * Square Point index:
    *  0   T   1
    *  L       R
    *  3   B   2
    */

    /* left wall L */
    if (mWalls[3])
    {
        vertexIndex->push_back(firstIndex);
        vertexIndex->push_back(firstIndex + 3);
    }

    /* top wall T */
    if (mWalls[2])
    {
        vertexIndex->push_back(firstIndex);
        vertexIndex->push_back(firstIndex + 1);
    }

    /* right wall R */
    if (mWalls[1])
    {
        vertexIndex->push_back(firstIndex + 1);
        vertexIndex->push_back(firstIndex + 2);
    }

    /* bottom wall */
    if (mWalls[0])
    {
        vertexIndex->push_back(firstIndex + 2);
        vertexIndex->push_back(firstIndex + 3);
    }
};

void Cell::appendVertexData(std::vector<MyVertexAttribs> *vertexData, float ratio, float paddingX, float paddingY)
{
    MyVertexAttribs v;

    for (int i = 0; i < 4; i++)
    {
        /* FIXME: Y-axis are reversed, but this should be transform work. */
        v.setXYZW(mVertex[i].getX() * ratio + paddingX, mVertex[i].getY() * ratio * -1.0f - paddingY, 0, 1);
        vertexData->push_back(v);
    }
};

void Cell::blockWall(int wall)
{
    mWallsUnblocked[wall] = false;
}