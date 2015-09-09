#include <string>
#include <fstream>

#include <SFML/Graphics.hpp>

#include "Cell.cpp"

using namespace std;

class Maze : public sf::Drawable, public sf::Transformable
{
    /* Size of the Maze */
    int mColumns, mRows;

    /* Positions of the Start and End Cells */
    int mStartCellRow, mStartCellColumn, mEndCellRow, mEndCellColumn;

    /* Offsets */
    int mLeftOffset, mTopOffset;
    float OFFSET = 0.01;

    /* Ratio constant */
    int mRatioWidth, mRatioHeigth;
    float RATIO = 0.98;

    /* Filename */
    string mFilename;

    /* Maze sizes */
    int mWidth, mHeigth;

    /* Cells */
    vector<Cell> mCells;

    void loadMaze()
    {
        ifstream infile(mFilename);

        // Read the number of Columns and Rows
        infile >> mRows;
        infile >> mColumns;

        updateRatio();

        // Read the star and end cells positions
        infile >> mStartCellRow >> mStartCellColumn >> mEndCellRow >> mEndCellColumn;

        mCells.reserve(mRows * mColumns);

        for (int y = 0; y < mRows; y++)
        {
            for (int x = 0; x < mColumns; x++)
            {
                int tempBitset;
                infile >> tempBitset;

                mCells.push_back(Cell(x, y, bitset<4>(tempBitset), mRatioHeigth, mRatioWidth));
            }
        }
    }

    void updateRatio() {
        mRatioWidth = mWidth * RATIO / mColumns;
        mRatioHeigth = mHeigth * RATIO / mRows;
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        for (int i = 0; i < mRows * mColumns; ++i)
        {
            mCells[i].draw(target, states);
        }
    }

public:

    Maze(string filename, int width, int heigth) {
        mFilename = filename;
        mWidth = width;
        mHeigth = heigth;

        loadMaze();
    }

    void update(int x1, int y1, int x2, int y2)
    {
        for (int i = 0; i < mCells.size(); i++)
        {
            mCells[i].update(x1, y1, x2, y2);
        }
    }

};