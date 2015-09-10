#include "Maze.h"

Maze::Maze(std::string filename, int width, int heigth) {
    mFilename = filename;
    mWidth = width;
    mHeigth = heigth;

    loadMaze();
}

bool Maze::isBoardWall(int x1, int y1, int x2, int y2)
{
    /* is both Xs is 0 or equal to mColumns, is board wall. */
    if (x1 == 0 && x2 == 0)
    {
        return true;
    }
    else if (y1 == 0 && y2 == 0)
    {
        return true;
    }
    else if (x1 == mColumns && x2 == mColumns)
    {
        return true;
    }
    else if (y1 == mRows && y2 == mRows)
    {
        return true;
    }

    return false;
}

void Maze::loadMaze()
{
    std::ifstream infile(mFilename);

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

            mCells.push_back(Cell(x, y, std::bitset<4>(tempBitset), mRatioHeigth, mRatioWidth));
        }
    }
}

void Maze::randomizeSide(int* wallX, int* wallY, int currentX, int currentY, int minX, int minY, int maxX, int maxY)
{
    do
    {
        *wallX = currentX;
        *wallY = currentY;

        /* select side */
        int add = Utils::randomInt(0, 1);

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

void Maze::randomWallVertex(int* wallX, int* wallY, int x1, int y1, int x2, int y2)
{
    *wallX = Utils::randomInt(x1, x2);
    *wallY = Utils::randomInt(y1, y2);

    /* I have to keep at least one coord equals to x1, y1, x2 or y2 and radomize the other one */
    int coord = Utils::randomInt(0, 3);

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

void Maze::removeTwoWalls(std::vector<int> cellsIndex)
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
        do
        {
            randomWallVertex(&firstWallX1, &firstWallY1, firstX, firstY, secondX, secondY);

            /* radomize second point of the first wall and make sure it is diferent from the first point */
            randomizeSide(&firstWallX2, &firstWallY2, firstWallX1, firstWallY1, firstX, firstY, secondX, secondY);
        
            /* if first wall is BOARD WALL, do it again */
        } while (isBoardWall(firstWallX1, firstWallY1, firstWallX2, firstWallY2));

        do
        {
            do
            {
                /* randomize first point of the second wall */
                randomWallVertex(&secondWallX1, &secondWallY1, firstX, firstY, secondX, secondY);

                /* if it is the same points of the first wall, keep trying */
            } while ((secondWallX1 == firstWallX1 && secondWallY1 == firstWallY1) || (secondWallX1 == firstWallX2 && secondWallY1 == firstWallY2));

            /* randomize second point of the second wall */
            randomizeSide(&secondWallX2, &secondWallY2, secondWallX1, secondWallY1, firstX, firstY, secondX, secondY);

        /* if second wall is BOARD WALL, do it again */
        } while (isBoardWall(secondWallX1, secondWallY1, secondWallX2, secondWallY2));

        /* remove both walls */
        for (int i = 0; i < mCells.size(); i++)
        {
            mCells[i].removeWall(firstWallX1, firstWallY1, firstWallX2, firstWallY2);
            mCells[i].removeWall(secondWallX1, secondWallY1, secondWallX2, secondWallY2);
        }

    }
}

void Maze::updateRatio() {
    mRatioWidth = mWidth * RATIO / mColumns;
    mRatioHeigth = mHeigth * RATIO / mRows;
}

void Maze::save(std::string outputName)
{
    std::ofstream myfile;

    myfile.open(outputName);

    myfile << mRows << "\n";
    myfile << mColumns << "\n";

    myfile << mStartCellRow << " " << mStartCellColumn << " " << mEndCellRow << " " << mEndCellColumn << "\n";

    for (int y = 0; y < mRows; y++)
    {
        for (int x = 0; x < mColumns; x++)
        {

            myfile << mCells[y * mColumns + x].getWallsInInt() << " ";
        }

        myfile << "\n";
    }

    myfile.close();
}

void Maze::update(int x1, int y1, int x2, int y2)
{
    std::vector<int> updatedCellsIndex;

    for (int i = 0; i < mCells.size(); i++)
    {
        if (mCells[i].update(x1, y1, x2, y2))
        {
            updatedCellsIndex.push_back(i);
        }
    }

    removeTwoWalls(updatedCellsIndex);
}