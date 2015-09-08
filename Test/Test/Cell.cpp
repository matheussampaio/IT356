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
    sf::Color NOT_WALL_COLOR = sf::Color::Transparent;

    /* Base Coordinates of the Cells */
    int mX, mY;

    /* Offsets */
    int mLeftOffset, mTopOffset;

    /* Screen Ratio */
    int mRatioWidth, mRatioHeigth;

public:

    Cell(int y, int x, std::bitset<4> walls, int ratioHeigth, int ratioWidth)
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
        quad[0].position = sf::Vector2f(mX * mRatioHeigth, mY * mRatioHeigth);
        quad[1].position = sf::Vector2f(mX * mRatioHeigth, (mY + 1) * mRatioHeigth);

        // upper
        quad[2].position = sf::Vector2f(mX * mRatioWidth, mY * mRatioWidth);
        quad[3].position = sf::Vector2f((mX + 1) * mRatioWidth, mY * mRatioWidth);

        // right
        quad[4].position = sf::Vector2f((mX + 1) * mRatioHeigth, mY * mRatioHeigth);
        quad[5].position = sf::Vector2f((mX + 1) * mRatioHeigth, (mY + 1) * mRatioHeigth);

        // bottom
        quad[6].position = sf::Vector2f((mX + 1) * mRatioWidth, (mY + 1) * mRatioWidth);
        quad[7].position = sf::Vector2f(mX * mRatioWidth, (mY + 1) * mRatioWidth);

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