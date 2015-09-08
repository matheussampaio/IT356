#include <iostream>
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

/* Maze Instance */
Maze mMaze("maze-5x10.txt", 800, 800);

/* Define if the screen should be updated. */
bool mDrawScreen = true;

// create the window
sf::RenderWindow mWindowInstance(sf::VideoMode(800, 800), "Maze");

int main(int argc, char *argv[])
{
    mMaze.setPosition(10, 10);

    // run the program as long as the window is open
    while (mWindowInstance.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (mWindowInstance.pollEvent(event))
        {
            processEvent(event);
        }

        if (mDrawScreen)
        {
            mDrawScreen = false;

            if (mWindowInstance.isOpen()) {
                display();
            }

        }

    }

    return 0;
}

void display() {
    // clear the window with white color
    mWindowInstance.clear(sf::Color::White);

    mWindowInstance.draw(mMaze);

    // end the current frame
    mWindowInstance.display();
}

void processEvent(sf::Event event)
{
    // "close requested" event: we close the window
    if (event.type == sf::Event::Closed)
    {
        mWindowInstance.close();
    }
    else if (event.type == sf::Event::Resized)
    {
        mDrawScreen = true;
    }
    else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        processLeftMousePressed(event);
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
    {
        processLeftMouseReleased(event);
    }
}

void processLeftMousePressed(sf::Event event)
{
    cout << "left mouse button pressed" << endl;
    std::cout << "mouse x: " << event.mouseButton.x << std::endl;
    std::cout << "mouse y: " << event.mouseButton.y << std::endl;
}

void processLeftMouseReleased(sf::Event event)
{
    cout << "left mouse button released" << endl;
    std::cout << "mouse x: " << event.mouseButton.x << std::endl;
    std::cout << "mouse y: " << event.mouseButton.y << std::endl;
}