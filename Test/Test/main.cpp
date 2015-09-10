#include <iostream>
#include <algorithm>

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include "Maze.h"

using namespace std;

/* Methods Prototypes */
void display(sf::RenderWindow *window);
void processEvent(sf::Event event, sf::RenderWindow &window);
void processLeftMousePressed(sf::Event event);
void processLeftMouseReleased(sf::Event event);
void processMouseMoved(sf::Event event);
void drawSquare(sf::RenderWindow *window);
void updateMaze();

/* Maze Instance */
Maze mMaze("maze-edited.txt", 800, 800);

/* Define if the screen should be updated. */
bool mDrawScreen = true;

/* Keep track if the left mouse button is pressed */
bool mLeftBtnMousePressed = false;

/* Keep track of the positions of mouse */
int mLeftBtnStartX, mLeftBtnStartY, mLeftBtnCurrentX, mLeftBtnCurrentY;

// create the window
sf::RectangleShape mSquare;

int main(int argc, char *argv[])
{

    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 32;
    contextSettings.majorVersion = 4;
    contextSettings.minorVersion = 0;

    sf::RenderWindow window(sf::VideoMode(800, 800), "Maze", sf::Style::Default, contextSettings);

    mMaze.setPosition(10, 10);
    window.setFramerateLimit(30);

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            processEvent(event, window);
        }

        if (window.isOpen()) {
            display(&window);
        }
    }

    return 0;
}

void display(sf::RenderWindow *window) {
    // clear the window with white color
    window->clear(sf::Color::White);

    window->draw(mMaze);

    if (mLeftBtnMousePressed)
    {
        drawSquare(window);
    }

    // end the current frame
    window->display();
}

void processEvent(sf::Event event, sf::RenderWindow &window)
{
    // "close requested" event: we close the window
    if (event.type == sf::Event::Closed)
    {
        mMaze.save("maze-20x20.txt");

        window.close();
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

void drawSquare(sf::RenderWindow *window)
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

    window->draw(lines);
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