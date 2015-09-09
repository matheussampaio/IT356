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

    void removeTwoWalls(vector<int> cellsIndex)
    {
        if (cellsIndex.size() > 1)
        {
            /* count walls in updateds cells */
            int firstX, firstY, secondX, secondY, firstWallX1, firstWallY1, firstWallX2,
                firstWallY2, secondWallX1, secondWallY1, secondWallX2, secondWallY2;

            /* min coords */
            firstX = mCells[cellsIndex[0]].getX();
            firstY = mCells[cellsIndex[0]].getY();

            /* max coords */
            secondX = mCells[cellsIndex[cellsIndex.size() - 1]].getX() + 1;
            secondY = mCells[cellsIndex[cellsIndex.size() - 1]].getY() + 1;

            /* radomize first point of the first wall */
            randomWallVertex(&firstWallX1, &firstWallY1, firstX, firstY, secondX, secondY);

            /* radomize second point of the first wall and make sure it is diferent from the first point */
            randomizeSide(&firstWallX2, &firstWallY2, firstWallX1, firstWallY1, firstX, firstY, secondX, secondY);

            do
            {   
                /* randomize first point of the second wall */
                randomWallVertex(&secondWallX1, &secondWallY1, firstX, firstY, secondX, secondY);

                /* if it is the same points of the first wall, keep trying */
            } while ((secondWallX1 == firstWallX1 && secondWallY1 == firstWallY1) || (secondWallX1 == firstWallX2 && secondWallY1 == firstWallY2));
            
            /* randomize second point of the second wall */
            randomizeSide(&secondWallX2, &secondWallY2, secondWallX1, secondWallY1, firstX, firstY, secondX, secondY);

            cout << firstX << "," << firstY << " - " << secondX << "," << secondY << endl;
            cout << firstWallX1 << "," << firstWallY1 << " --> " << firstWallX2 << "," << firstWallY2 << endl;
            cout << secondWallX1 << "," << secondWallY1 << " --> " << secondWallX2 << "," << secondWallY2 << endl;

            /* remove both walls */
            for (int i = 0; i < mCells.size(); i++)
            {
                mCells[i].removeWall(firstWallX1, firstWallY1, firstWallX2, firstWallY2);
                mCells[i].removeWall(secondWallX1, secondWallY1, secondWallX2, secondWallY2);
            }

        }
    }

    void randomWallVertex(int* wallX, int* wallY, int x1, int y1, int x2, int y2)
    {
        *wallX = randomInt(x1, x2);
        *wallY = randomInt(y1, y2);
        
        /* I have to keep at least one coord equals to x1, y1, x2 or y2 and radomize the other one */
        int coord = randomInt(0, 3);

        /* keep x1 */
        if (coord == 0)
        {
            *wallX = x1;
        }
        /* keep y1 */
        else if (coord == 1)
        {
            *wallY = y1;
        }
        /* keep x2 */
        else if (coord == 2)
        {
            *wallX = x2;
        }
        /* keep y2 */
        else if (coord == 3)
        {
            *wallY = y2;
        }

    }

    int randomInt(int minNumber, int maxNumber)
    {
        return rand() % (maxNumber + 1 - minNumber) + minNumber;
    }

    void randomizeSide(int* wallX, int* wallY, int currentX, int currentY, int minX, int minY, int maxX, int maxY)
    {
        do
        {
            *wallX = currentX;
            *wallY = currentY;

            /* select side */
            int add = randomInt(0, 1);

            if (*wallX == minX || *wallX == maxX)
            {
                if (add)
                {
                    *wallY = *wallY + 1;
                }
                else
                {
                    *wallY = *wallY - 1;
                }
            }
            else
            {
                if (add)
                {
                    *wallX = *wallX + 1;
                }
                else
                {
                    *wallX = *wallX - 1;
                }
            }

        } while (!(minX <= *wallX && *wallX <= maxX && minY <= *wallY && *wallY <= maxY));
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
        int countWalls = 0;

        vector<int> updatedCellsIndex;

        for (int i = 0; i < mCells.size(); i++)
        {
            if (mCells[i].update(x1, y1, x2, y2))
            {
                updatedCellsIndex.push_back(i);
            }
        }

        removeTwoWalls(updatedCellsIndex);
    }

    void save(string outputName)
    {
        cout << "saving maze" << endl;
    }

};