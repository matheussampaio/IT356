#ifndef MAZE_H
#define MAZE_H

#include "Cell.h"
#include "Utils.h"

#include <string>
#include <fstream>
#include <stdlib.h>

#include <SFML/Graphics.hpp>

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
    std::string mFilename;

    /* Maze sizes */
    int mWidth, mHeigth;

    /* Cells */
    std::vector<Cell> mCells;

    bool isBoardWall(int x1, int y1, int x2, int y2);

    void loadMaze();

    void randomizeSide(int* wallX, int* wallY, int currentX, int currentY, int minX, int minY, int maxX, int maxY);

    void randomWallVertex(int* wallX, int* wallY, int x1, int y1, int x2, int y2);

    void removeTwoWalls(std::vector<int> cellsIndex);
    
    void updateRatio();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        for (int i = 0; i < mRows * mColumns; ++i)
        {
            mCells[i].draw(target, states);
        }
    }

public:

    Maze(std::string filename, int width, int heigth);

    void save(std::string outputName);

    void update(int x1, int y1, int x2, int y2);

};

#endif