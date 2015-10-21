#ifndef MAZE_H
#define MAZE_H

#include <string>
#include <fstream>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include "VertexAttribs.h"
#include "Cell.h"
#include "Utils.h"

#include <SFML/Graphics.hpp>

class Maze : public sf::Drawable, public sf::Transformable
{
    /* Size of the Maze */
    int mColumns, mRows;

    /* Positions of the Start and End Cells */
    int mStartCellRow, mStartCellColumn, mEndCellRow, mEndCellColumn;

    /* Offsets */
    float mPaddingX, mPaddingY;

    /* Ratio constant */
    float mRatio, MAX_SIZE = 0.98f;

    /* Filename */
    std::string mFilename;

    /* Maze sizes */
    int mWidth, mHeigth;

    /* Cells */
    std::vector<Cell> mCells;

    std::vector<GLuint> mVertexIndex;
    std::vector<MyVertexAttribs> mVertexData;

    bool isBoardWall(int x1, int y1, int x2, int y2);

    void loadMaze();

    void randomizeSide(int* wallX, int* wallY, int currentX, int currentY, int minX, int minY, int maxX, int maxY);

    void randomWallVertex(int* wallX, int* wallY, int x1, int y1, int x2, int y2);

    void removeTwoWalls(std::vector<int> cellsIndex);
    
    void updateRatio();

    bool isUpdateValid(float x1, float y1, float x2, float y2);

    void blockBoardWalls();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {

    }

public:

    Maze(std::string filename, int width, int heigth);

    void save(std::string outputName);

    bool update(float x1, float y1, float x2, float y2);
    void resize(float width, float heigth);

    /* ACESSORS */

    std::vector<GLuint> getVertexIndex();
    std::vector<MyVertexAttribs> getVertexData();

    int getRows() { return mRows; };
    int getColumns() { return mColumns; };

};

#endif