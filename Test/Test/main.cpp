#include <iostream>
#include <algorithm>

#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

#include "View.h"
#include "Maze.h"

using namespace std;

/* Methods Prototypes */
void init(sf::RenderWindow *window);
void resize(int w, int h);
void display(sf::RenderWindow *window);
void processEvent(sf::Event event, sf::RenderWindow &window);
void processLeftMousePressed(sf::Event event);
void processLeftMouseReleased(sf::Event event);
void processMouseMoved(sf::Event event);
void drawSquare(sf::RenderWindow *window);
void updateMaze();

/* Maze Instance */
Maze mMaze("maze-20x20.txt", 800, 800);

/* Define if the screen should be updated. */
bool mDrawScreen = true;

/* Keep program running */
bool mRunning = true;

/* Keep track if the left mouse button is pressed */
bool mLeftBtnMousePressed = false;

/* Keep track of the positions of mouse */
int mLeftBtnStartX, mLeftBtnStartY, mLeftBtnCurrentX, mLeftBtnCurrentY;

// create the window
sf::RectangleShape mSquare;

/* Our Main View */
View mView;

int t = 0;

int main(int argc, char *argv[])
{

    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 32;
    contextSettings.majorVersion = 4;
    contextSettings.minorVersion = 0;

    /* Creating window */
    sf::RenderWindow window(sf::VideoMode(800, 800), "Maze", sf::Style::Default, contextSettings);

    //glew currently has a bug that causes problems with 4.0. Setting this glew flag is the temporary solution for it
    glewExperimental = GL_TRUE;

    //initialize glew which initializes all the OpenGL extensions.
    if (glewInit() != GLEW_OK)
    {
        cerr << "Unable to initialize GLEW...exiting" << endl;
        return EXIT_FAILURE;
    }

    init(&window);
    
    // run the program as long as the window is open
    while (mRunning)
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            processEvent(event, window);
        }

        // clear the buffers
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw...

        // end the current frame (internally swaps the front and back buffers)
        display(&window);
    }

    return EXIT_SUCCESS;
}

void display(sf::RenderWindow *window) {
    // clear the window with white color
    // Draw using SFML
    //window->pushGLStates();
    //window->resetGLStates();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mView.draw();

    if (mLeftBtnMousePressed)
    {
        //drawSquare(window);
    }
    
    //window->popGLStates();

    // end the current frame
    window->display();

}

void processEvent(sf::Event event, sf::RenderWindow &window)
{
    // "close requested" event: we close the window
    if (event.type == sf::Event::Closed)
    {
        mMaze.save("maze-20x20.txt");

        // end the program
        mRunning = false;
    }

    if (event.type == sf::Event::Resized)
    {
        mDrawScreen = true;

        // adjust the viewport when the window is resized
        glViewport(0, 0, event.size.width, event.size.height);
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

void resize(int w, int h)
{
    //delegate to our view class.
    mView.resize(w, h);

    //sets the viewport to cover the entire area of the resized window
    //glViewport(leftx,topy,width,height)
    glViewport(0, 0, w, h);
}

void init(sf::RenderWindow *window)
{
    /* Some configurations */
    resize(800, 800);
    
    window->setFramerateLimit(30);

    glClearColor(1, 1, 1, 0); // set clear code to white

    mMaze.setPosition(10, 10);

    mMaze.getVertexData(mView.getVertexDataPointer());

    mMaze.getVertexIndex(mView.getVertexIndexPointer());

    mView.initialize();
}