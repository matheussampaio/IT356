#include <iostream>
#include <algorithm>

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include "Maze.cpp"

using namespace std;

/* Methods Prototypes */
void display();
void processEvent(sf::Event event);
void processLeftMousePressed(sf::Event event);
void processLeftMouseReleased(sf::Event event);
void processMouseMoved(sf::Event event);
void drawSquare();
void eraseSquare();
void updateMaze();

/* Maze Instance */
Maze mMaze("maze-20x20.txt", 800, 800);

/* Define if the screen should be updated. */
bool mDrawScreen = true;

/* Keep track if the left mouse button is pressed */
bool mLeftBtnMousePressed = false;

/* Keep track of the positions of mouse */
int mLeftBtnStartX, mLeftBtnStartY, mLeftBtnCurrentX, mLeftBtnCurrentY;

// create the window
sf::RenderWindow mWindowInstance(sf::VideoMode(800, 800), "Maze");
sf::RectangleShape mSquare;

int main(int argc, char *argv[])
{
    mMaze.setPosition(10, 10);
    mWindowInstance.setFramerateLimit(30);

    // run the program as long as the window is open
    while (mWindowInstance.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (mWindowInstance.pollEvent(event))
        {
            processEvent(event);
        }

        if (mWindowInstance.isOpen()) {
            display();
        }
    }

    return 0;
}

void display() {
    // clear the window with white color
    mWindowInstance.clear(sf::Color::White);

    mWindowInstance.draw(mMaze);

    if (mLeftBtnMousePressed)
    {
        drawSquare();
    }


    // end the current frame
    mWindowInstance.display();
}

void processEvent(sf::Event event)
{
    // "close requested" event: we close the window
    if (event.type == sf::Event::Closed)
    {
        mMaze.save("maze-edited.txt");

        mWindowInstance.close();
    }

    if (event.type == sf::Event::Resized)
    {
        mDrawScreen = true;
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        processLeftMousePressed(event);
    }

    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
    {
        processLeftMouseReleased(event);
    }

    if (event.type == sf::Event::MouseMoved)
    {
        processMouseMoved(event);
    }
}

void processLeftMousePressed(sf::Event event)
{
    mLeftBtnMousePressed = true;

    mLeftBtnStartX = event.mouseButton.x;
    mLeftBtnStartY = event.mouseButton.y;

    mLeftBtnCurrentX = event.mouseButton.x;
    mLeftBtnCurrentY = event.mouseButton.y;
}

void processLeftMouseReleased(sf::Event event)
{
    mLeftBtnMousePressed = false;

    updateMaze();
}

void processMouseMoved(sf::Event event)
{
    if (mLeftBtnMousePressed)
    {
        mLeftBtnCurrentX = event.mouseMove.x;
        mLeftBtnCurrentY = event.mouseMove.y;
    }
}

void drawSquare()
{
    //cout << mLeftBtnStartX << "," << mLeftBtnStartY << " - " << mLeftBtnCurrentX << "," << mLeftBtnCurrentY << endl;

    sf::VertexArray lines(sf::LinesStrip, 5);

    lines[0].position = sf::Vector2f(mLeftBtnStartX, mLeftBtnStartY);
    lines[0].color = sf::Color::Red;

    lines[1].position = sf::Vector2f(mLeftBtnCurrentX, mLeftBtnStartY);
    lines[1].color = sf::Color::Red;

    lines[2].position = sf::Vector2f(mLeftBtnCurrentX, mLeftBtnCurrentY);
    lines[2].color = sf::Color::Red;

    lines[3].position = sf::Vector2f(mLeftBtnStartX, mLeftBtnCurrentY);
    lines[3].color = sf::Color::Red;

    lines[4].position = sf::Vector2f(mLeftBtnStartX, mLeftBtnStartY);
    lines[4].color = sf::Color::Red;

    mWindowInstance.draw(lines);
}

void updateMaze()
{
    mMaze.update(
        min(mLeftBtnStartX, mLeftBtnCurrentX),
        min(mLeftBtnStartY, mLeftBtnCurrentY),
        max(mLeftBtnStartX, mLeftBtnCurrentX),
        max(mLeftBtnStartY, mLeftBtnCurrentY)
        );
}