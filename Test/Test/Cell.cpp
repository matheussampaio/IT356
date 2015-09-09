#include <bitset>
#include <iostream>

#include <SFML/Graphics.hpp>

class Cell
{

    /* Bits representing the walls*/
    std::bitset<4> mWalls;

    /* Quad Points of the Cells */
    sf::VertexArray mVertices;

    /* Color of the wall */
    sf::Color WALL_COLOR = sf::Color::Black;
    sf::Color NOT_WALL_COLOR = sf::Color::Transparent;

    /* Base Coordinates of the Cells */
    int mX, mY;

    /* Offsets */
    int mLeftOffset, mTopOffset;

    /* Screen Ratio */
    int mRatioWidth, mRatioHeigth;

    bool isPointInSquare(int pointX, int pointY, int squareX1, int squareY1, int squareX2, int squareY2)
    {
        if (squareX1 <= pointX && pointX <= squareX2 && squareY1 <= pointY && pointY <= squareY2)
        {
            return true;
        }

        return false;
    }

public:

    Cell(int x, int y, std::bitset<4> walls, int ratioHeigth, int ratioWidth)
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

    void refreshVertices()
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
    void refreshWalls()
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

    bool update(int x1, int y1, int x2, int y2)
    {
        /* check if any vertex are inside of the square */
        bool leftTopVertexInside = isPointInSquare(mX * mRatioWidth, mY * mRatioHeigth, x1, y1, x2, y2);
        bool rightTopVertexInside = isPointInSquare((mX + 1) * mRatioWidth, mY * mRatioHeigth, x1, y1, x2, y2);
        bool rightBottomVertexInside = isPointInSquare((mX + 1) * mRatioWidth, (mY + 1) * mRatioHeigth, x1, y1, x2, y2);
        bool leftBottomVertexInside = isPointInSquare(mX * mRatioWidth, (mY + 1) * mRatioHeigth, x1, y1, x2, y2);

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

    int getNumberOfWalls()
    {
        int count = 0;

        for (int i = 0; i < 4; i++)
        {
            if (mWalls[i])
            {
                count++;
            }
        }

        return count;
    }

    int openOneDoor(int wallNumber)
    {
        //std::cout << "open " << wallNumber << " door on cell " << mX << "," << mY << std::endl;

        //int count = 0;
        
        sf::Vertex* quad = &mVertices[0];

        for (int i = 0; i <= 3; i++)
        {
            
            if (mWalls[3 - i])
            {
                std::cout << "porta em " << mX << "," << mY << " lado " << 3 - i << std::endl;

                wallNumber--;

                std::cout << "wallNumber: " << wallNumber << std::endl;

                if (wallNumber == 0)
                {
                    mWalls[3 - i] = 0;
                    std::cout << "PORTA ABERTA: " << 3 - i << std::endl;
                    
                    quad[i * 2].color = sf::Color::Green;
                    quad[(i * 2) + 1].color = sf::Color::Green;
                    
                    //refreshWalls();

                    return -1;
                }
                
            }
        }

        return wallNumber;
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(mVertices, states);
    }


};