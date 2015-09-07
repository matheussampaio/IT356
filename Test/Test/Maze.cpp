#include <iostream>
#include <string>
#include <fstream>
#include <bitset>

#include <SFML/Graphics.hpp>

using namespace std;

class Maze : public sf::Drawable, public sf::Transformable
{
public:

    bool init(string filename, int screenWidth, int screenHeigth)
    {
        load(filename);

        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::Lines);
        m_vertices.resize(mColumns * mRows * 8);
        
        int ratioHeigth = screenHeigth * 0.98 / mRows;
        int ratioWidth = screenWidth * 0.98 / mColumns;

        // populate the vertex array, with one quad per tile
        for (int i = 0; i < mColumns; i++)
        {
            for (int j = 0; j < mRows; j++)
            {
                // get a pointer to the current tile's quad
                sf::Vertex* quad = &m_vertices[(i + j * mColumns) * 8];

                // left
                quad[0].position = sf::Vector2f(i * ratioHeigth, (j + 1) * ratioHeigth);
                quad[1].position = sf::Vector2f(i * ratioHeigth, j * ratioHeigth);

                // upper
                quad[2].position = sf::Vector2f(i * ratioWidth, j * ratioWidth);
                quad[3].position = sf::Vector2f((i + 1) * ratioWidth, j * ratioWidth);

                // right
                quad[4].position = sf::Vector2f((i + 1) * ratioHeigth, j * ratioHeigth);
                quad[5].position = sf::Vector2f((i + 1) * ratioHeigth, (j + 1) * ratioHeigth);

                // bottom
                quad[6].position = sf::Vector2f((i + 1) * ratioWidth, (j + 1) * ratioWidth);
                quad[7].position = sf::Vector2f(i * ratioWidth, (j + 1) * ratioWidth);

                for (int z = 0; z < 4; z++)
                {
                    if (mCells[(j * mColumns) + i][3 - z]) {
                        quad[z * 2].color = sf::Color::Black;
                        quad[(z * 2) + 1].color = sf::Color::Black;
                    }
                }
            }
        }

        return true;
    }

private:

    void load(string filename)
    {
        ifstream infile(filename);

        // Read the number of Columns and Rows
        infile >> mColumns >> mRows;

        // Read the star and end cells positions
        infile >> mStartCellRow >> mStartCellColumn >> mEndCellRow >> mEndCellColumn;

        mCells.reserve(mRows * mColumns);

        for (int i = 0; i < mColumns; i++)
        {
            for (int j = 0; j < mRows; j++)
            {
                int tempBitset;
                infile >> tempBitset;
                mCells.push_back(bitset<4>(tempBitset));
            }
        }
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // draw the vertex array
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
    
    int mColumns, mRows, mStartCellRow, mStartCellColumn, mEndCellRow, mEndCellColumn;

    vector<bitset<4>> mCells;
    
};