#ifndef CELL_H
#define CELL_H

#include <bitset>
#include <iostream>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GL/gl.h>

#include "VertexAttribs.h"
#include "Utils.h"

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
    int mX, mY, mIndex;

    /* Offsets */
    int mLeftOffset, mTopOffset;

    /* Screen Ratio */
    int mRatioWidth, mRatioHeigth;

public:

    Cell(int index, int x, int y, std::bitset<4> walls, int ratioHeigth, int ratioWidth);

    void refreshVertices();
    void refreshWalls();

    bool update(int x1, int y1, int x2, int y2);

    void removeWall(int x1, int y1, int x2, int y2);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(mVertices, states);
    };

    void appendVertexIndex(std::vector<GLuint> *vertexIndex);

    void appendVertexData(std::vector<VertexAttribs> *vertexData);

    int getX() { return mX; };
    int getY() { return mY; };
    int getWallsInInt() { return Utils::bitsetToInt(mWalls); };

};

#endif