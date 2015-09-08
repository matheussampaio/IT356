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
void processMouseMoved(sf::Event event);

/* Maze Instance */
Maze mMaze("maze-20x20.txt", 800, 800);

/* Define if the screen should be updated. */
bool mDrawScreen = true;

/* Keep track if the left mouse button is pressed */
bool mLeftBtnMousePressed = false;

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
    cout << "left mouse button pressed" << endl;
    std::cout << "mouse x: " << event.mouseButton.x << std::endl;
    std::cout << "mouse y: " << event.mouseButton.y << std::endl;
}

void processLeftMouseReleased(sf::Event event)
{
    mLeftBtnMousePressed = false;
    cout << "left mouse button released" << endl;
    std::cout << "mouse x: " << event.mouseButton.x << std::endl;
    std::cout << "mouse y: " << event.mouseButton.y << std::endl;
}

void processMouseMoved(sf::Event event)
{
    if (mLeftBtnMousePressed)
    {
        std::cout << "mouse x: " << event.mouseMove.x << std::endl;
        std::cout << "mouse y: " << event.mouseMove.y << std::endl;
    }
}