#include "Maze.h"

#include <algorithm>

Maze::Maze(std::string filename, int width, int heigth) {
    mFilename = filename;
    mWidth = width;
    mHeigth = heigth;

    loadMaze();
    updateRatio();

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

    // Read the star and end cells positions
    infile >> mStartCellRow >> mStartCellColumn >> mEndCellRow >> mEndCellColumn;

    mCells.reserve(mRows * mColumns);

    for (int y = 0; y < mRows; y++)
    {
        for (int x = 0; x < mColumns; x++)
        {
            int tempBitset;
            infile >> tempBitset;

            mCells.push_back(Cell((y * mColumns + x) * 4, x, y, std::bitset<4>(tempBitset)));
        }
    }

    blockBoardWalls();
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
        std::cout << "removing two walls" << std::endl;

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

        std::printf("Removing wall: %d,%d - %d,%d\n", firstWallX1, firstWallY1, firstWallX2, firstWallY2);
        std::printf("Removing wall: %d,%d - %d,%d\n", secondWallX1, secondWallY1, secondWallX2, secondWallY2);

        /* remove both walls */
        for (int i = 0; i < mCells.size(); i++)
        {
            mCells[i].removeWall(firstWallX1, firstWallY1, firstWallX2, firstWallY2);
            mCells[i].removeWall(secondWallX1, secondWallY1, secondWallX2, secondWallY2);
        }
    }
}

void Maze::updateRatio() {
    mRatio = std::min(mWidth * MAX_SIZE / mColumns, mHeigth * MAX_SIZE / mRows);

    mPaddingX = mWidth * 0.01f;
    mPaddingY = mHeigth * 0.01f;
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

    std::cout << "Maze saved: " << outputName << std::endl;

}

bool Maze::update(float x1, float y1, float x2, float y2)
{
    x1 = (x1 - mPaddingX) / mRatio;
    x2 = (x2 - mPaddingX) / mRatio;
    y1 = (y1 - mPaddingY) / mRatio;
    y2 = (y2 - mPaddingY) / mRatio;
    
    if (Maze::isUpdateValid(x1, y1, x2, y2))
    {
        std::printf("Updating maze: %.2f,%.2f --> %.2f,%.2f\n", x1, y1, x2, y2);

        std::vector<int> updatedCellsIndex;

        for (int i = 0; i < mCells.size(); i++)
        {
            if (mCells[i].update(x1, y1, x2, y2))
            {
                updatedCellsIndex.push_back(i);
            }
        }

        removeTwoWalls(updatedCellsIndex);

        return true;
    }

    std::printf("Ignoring updating maze: %.2f,%.2f -> %.2f,%.2f\n", x1, y1, x2, y2);

    return false;
}

bool Maze::isUpdateValid(float x1, float y1, float x2, float y2)
{

    if (x1 <= 0 || x1 >= mColumns)
    {
        return false;
    }

    if (x2 <= 0 || x2 >= mColumns)
    {
        return false;
    }

    if (y1 <= 0 || y1 >= mRows)
    {
        return false;
    }

    if (y2 <= 0 || y2 >= mRows)
    {
        return false;
    }

    return true;
}

std::vector<MyVertexAttribs> Maze::getVertexData()
{
    std::vector<MyVertexAttribs> vertexdata;

    for (int i = 0; i < mCells.size(); i++)
    {
        mCells[i].appendVertexData(&vertexdata, mRatio, mPaddingX, mPaddingY);
    }

    return vertexdata;
}

std::vector<GLuint> Maze::getVertexIndex()
{
    std::vector<GLuint> vertexIndex;

    for (int i = 0; i < mCells.size(); i++)
    {
        mCells[i].appendVertexIndex(&vertexIndex);
    }

    return vertexIndex;
}

void Maze::resize(float width, float heigth)
{
    mWidth = width;
    mHeigth = heigth;

    updateRatio();
};

void Maze::blockBoardWalls()
{
    for (int i = 0; i < mCells.size(); i++)
    {
        if (mCells[i].getX() == 0)
        {
            mCells[i].blockWall(3);
        }

        if (mCells[i].getY() == 0)
        {
            mCells[i].blockWall(2);
        }

        if (mCells[i].getX() == mColumns - 1)
        {
            mCells[i].blockWall(1);
        }

        if (mCells[i].getY() == mRows - 1)
        {
            mCells[i].blockWall(0);
        }
    }
}