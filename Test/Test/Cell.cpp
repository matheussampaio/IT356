#include "Cell.h"
#include "Utils.h"

Cell::Cell(int index, int x, int y, std::bitset<4> walls, int ratioHeigth, int ratioWidth)
{
    mX = x;
    mY = y;
    mWalls = walls;
    mIndex = index;

    mRatioHeigth = ratioHeigth;
    mRatioWidth = ratioWidth;

    /* FIXME: Y-axis are reversed, but this should be transform work. */

    // left - top : x, y
    mVertex[0].setXYZW(mX * mRatioWidth, -mY * mRatioHeigth, 0, 1);

    // right - top : x + 1, y
    mVertex[1].setXYZW((mX + 1) * mRatioWidth, -mY * mRatioHeigth, 0, 1);

    // right - bottom : x + 1, y + 1
    mVertex[2].setXYZW((mX + 1) * mRatioWidth, -(mY + 1) * mRatioHeigth, 0, 1);

    // left - bottom : x, y + 1
    mVertex[3].setXYZW(mX * mRatioWidth, -(mY + 1) * mRatioHeigth, 0, 1);
}

bool Cell::update(int x1, int y1, int x2, int y2)
{
    /* check if any vertex are inside of the square */
    bool leftTopVertexInside = Utils::isPointInSquare(mX * mRatioWidth, mY * mRatioHeigth, x1, y1, x2, y2);
    bool rightTopVertexInside = Utils::isPointInSquare((mX + 1) * mRatioWidth, mY * mRatioHeigth, x1, y1, x2, y2);
    bool rightBottomVertexInside = Utils::isPointInSquare((mX + 1) * mRatioWidth, (mY + 1) * mRatioHeigth, x1, y1, x2, y2);
    bool leftBottomVertexInside = Utils::isPointInSquare(mX * mRatioWidth, (mY + 1) * mRatioHeigth, x1, y1, x2, y2);

    /* if any vertex are inside of the square */
    if (leftTopVertexInside || leftBottomVertexInside || rightTopVertexInside || rightBottomVertexInside)
    {
        /* if at least one vertex of the wall is inside of the square,remove THAT wall, otherwise, ADD that wall. */
        mWalls[3] = !(leftTopVertexInside || leftBottomVertexInside);
        mWalls[2] = !(rightTopVertexInside || leftTopVertexInside);
        mWalls[1] = !(rightTopVertexInside || rightBottomVertexInside);
        mWalls[0] = !(leftBottomVertexInside || rightBottomVertexInside);

        return true;
    }

    return false;
}

void Cell::removeWall(int x1, int y1, int x2, int y2)
{
    GLfloat vX1, vY1, vX2, vY2;

    x1 *= mRatioWidth;
    x2 *= mRatioHeigth;

    y1 *= mRatioHeigth;
    y2 *= mRatioHeigth;

    for (int i = 0; i <= 3; i++)
    {
        vX1 = mVertex[i].getX();
        vY1 = mVertex[i].getY();

        vX2 = mVertex[(i + 1) % 3].getX();
        vY2 = mVertex[(i + 1) % 3].getY();

        if ((Utils::isPointEqual(x1, y1, vX1, vY1) && Utils::isPointEqual(x2, y2, vX2, vY2)) || (Utils::isPointEqual(x1, y1, vX2, vY2) && Utils::isPointEqual(x2, y2, vX1, vY1)))
        {
            mWalls[3 - i] = 0;
        }
    }
}

void Cell::appendVertexIndex(std::vector<GLuint> *vertexIndex) {

    int firstIndex = mIndex;

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

void Cell::appendVertexData(std::vector<VertexAttribs> *vertexData)
{
    // left - top : x, y
    vertexData->push_back(mVertex[0]);

    // right - top : x + 1, y
    vertexData->push_back(mVertex[1]);

    // right - bottom : x + 1, y + 1
    vertexData->push_back(mVertex[2]);

    // left - bottom : x, y + 1
    vertexData->push_back(mVertex[3]);
};