#include <string>
#include <fstream>
#include <stdlib.h>

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

    void openRandomWall(vector<int> cellsIndex)
    {
        if (cellsIndex.size() > 0)
        {
            /* count walls in updateds cells */
            int count = 0;

            for (int i : cellsIndex)
            {
                count += mCells[i].getNumberOfWalls();
            }

            int wallNumber = rand() % count;

            cout << "-------" << endl << "conta " << wallNumber  << " e abre." << endl;

            count = 0;
            for (int i : cellsIndex)
            {
                wallNumber = mCells[i].openOneDoor(wallNumber);

                if (wallNumber <= 0) {
                    cout << "end" << endl;
                    return;
                }
            }
        }
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
        int countWalls = 0, countCells = 0, temp = 0;

        vector<int> updatedCellsIndex;

        for (int i = 0; i < mCells.size(); i++)
        {
            if (mCells[i].update(x1, y1, x2, y2))
            {
                updatedCellsIndex.push_back(i);
            }
        }

        openRandomWall(updatedCellsIndex);
        openRandomWall(updatedCellsIndex);
    }

    void save(string outputName)
    {
        cout << "saving maze" << endl;
    }

};