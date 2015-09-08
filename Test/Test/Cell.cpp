#include <iostream>
#include <bitset>

#include <SFML/Graphics.hpp>

class Cell
{

    /* Bits representing the walls*/
    std::bitset<4> mWalls;
    
    /* Quad Points of the Cells */
    sf::VertexArray mVertices;

    /* Color of the wall */
    sf::Color WALL_COLOR = sf::Color::Black;
    sf::Color NOT_WALL_COLOR = sf::Color::Red;

    /* Base Coordinates of the Cells */
    int mY, mX;

    /* Offsets */
    int mLeftOffset, mTopOffset;

    /* Screen Ratio */
    int mRatioWidth, mRatioHeigth;

public:

    Cell(int x, int y, std::bitset<4> walls, int ratioHeigth, int ratioWidth)
    {
        mX = x;
        mY = y;
        mWalls = walls;
        mRatioHeigth = 50; // ratioHeigth;
        mRatioWidth = 50;// ratioWidth;

        mVertices.setPrimitiveType(sf::Lines);
        mVertices.resize(8);

        refreshVertices();
        refreshWalls();
    }

    void refreshVertices()
    {
        sf::Vertex* quad = &mVertices[0];

        // left
        quad[0].position = sf::Vector2f(mY * mRatioHeigth, mX * mRatioHeigth);
        quad[1].position = sf::Vector2f(mY * mRatioHeigth, (mX + 1) * mRatioHeigth);

        // upper
        quad[2].position = sf::Vector2f(mY * mRatioWidth, mX * mRatioWidth);
        quad[3].position = sf::Vector2f((mY + 1) * mRatioWidth, mX * mRatioWidth);

        // right
        quad[4].position = sf::Vector2f((mY + 1) * mRatioHeigth, mX * mRatioHeigth);
        quad[5].position = sf::Vector2f((mY + 1) * mRatioHeigth, (mX + 1) * mRatioHeigth);

        // bottom
        quad[6].position = sf::Vector2f((mY + 1) * mRatioWidth, (mX + 1) * mRatioWidth);
        quad[7].position = sf::Vector2f(mY * mRatioWidth, (mX + 1) * mRatioWidth);

    }

    void refreshWalls()
    {
        sf::Vertex* quad = &mVertices[0];

        for (int i = 0; i <= 3; i++)
        {
            /* Bits are in reverse order */
            if (mWalls[3 - i])
            {
                quad[i * 2].color = sf::Color::Black;
                quad[(i * 2) + 1].color = sf::Color::Black;
            }
            else
            {
                quad[i * 2].color = sf::Color::Transparent;
                quad[(i * 2) + 1].color = sf::Color::Transparent;
            }
        }
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(mVertices, states);
    }
};