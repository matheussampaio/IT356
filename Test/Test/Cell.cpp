#include "Cell.h"
#include "Utils.h"

Cell::Cell(int x, int y, std::bitset<4> walls, int ratioHeigth, int ratioWidth)
{
    mX = x;
    mY = y;
    mWalls = walls;

    mRatioHeigth = ratioHeigth;
    mRatioWidth = ratioWidth;

    mVertices.setPrimitiveType(sf::Lines);
    mVertices.resize(8);

    refreshVertices();
    refreshWalls();
}

void Cell::refreshVertices()
{
    sf::Vertex* quad = &mVertices[0];

    // left
    quad[0].position = sf::Vector2f(mX * mRatioWidth, mY * mRatioHeigth);
    quad[1].position = sf::Vector2f(mX * mRatioWidth, (mY + 1) * mRatioHeigth);

    // upper
    quad[2].position = sf::Vector2f(mX * mRatioWidth, mY * mRatioHeigth);
    quad[3].position = sf::Vector2f((mX + 1) * mRatioWidth, mY * mRatioHeigth);

    // right
    quad[4].position = sf::Vector2f((mX + 1) * mRatioWidth, mY * mRatioHeigth);
    quad[5].position = sf::Vector2f((mX + 1) * mRatioWidth, (mY + 1) * mRatioHeigth);

    // bottom
    quad[6].position = sf::Vector2f((mX + 1) * mRatioWidth, (mY + 1) * mRatioHeigth);
    quad[7].position = sf::Vector2f(mX * mRatioWidth, (mY + 1) * mRatioHeigth);

}

/* Refresh WALL colors */
void Cell::refreshWalls()
{
    sf::Vertex* quad = &mVertices[0];

    for (int i = 0; i <= 3; i++)
    {
        /* Bits are in reverse order */
        if (mWalls[3 - i])
        {
            quad[i * 2].color = WALL_COLOR;
            quad[(i * 2) + 1].color = WALL_COLOR;
        }
        else
        {
            quad[i * 2].color = NOT_WALL_COLOR;
            quad[(i * 2) + 1].color = NOT_WALL_COLOR;
        }
    }
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

        refreshWalls();

        return true;
    }

    return false;
}

void Cell::removeWall(int x1, int y1, int x2, int y2)
{
    sf::Vertex* quad = &mVertices[0];

    int vX1, vY1, vX2, vY2;

    x1 *= mRatioWidth;
    x2 *= mRatioHeigth;

    y1 *= mRatioHeigth;
    y2 *= mRatioHeigth;

    for (int i = 0; i < 4; i++)
    {
        vX1 = quad[i * 2].position.x;
        vY1 = quad[i * 2].position.y;

        vX2 = quad[(i * 2) + 1].position.x;
        vY2 = quad[(i * 2) + 1].position.y;

        if ((Utils::isPointEqual(x1, y1, vX1, vY1) && Utils::isPointEqual(x2, y2, vX2, vY2)) || (Utils::isPointEqual(x1, y1, vX2, vY2) && Utils::isPointEqual(x2, y2, vX1, vY1)))
        {
            mWalls[3 - i] = 0;

            refreshWalls();
        }
    }
}