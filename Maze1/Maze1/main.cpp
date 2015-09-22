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
void drawSquare();
void updateMaze();


string FILENAME_INPUT = "maze-500x500.txt";
string FILENAME_OUTPUT = "maze-edited.txt";

int INIT_WIDTH = 800, INIT_HEIGHT = 800;

/* Maze Instance */
Maze mMaze(FILENAME_INPUT, INIT_WIDTH, INIT_HEIGHT);

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
View mViewSquare;

/* Define is data and index should update */
bool mDataOutdated = true, mIndexOutdated = true;

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

        display(&window);
    }

    return EXIT_SUCCESS;
}

void display(sf::RenderWindow *window) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (mDataOutdated)
    {
        mView.setVertexData(mMaze.getVertexData());

        mDataOutdated = false;
    }

    if (mIndexOutdated)
    {
        mView.setVertexIndex(mMaze.getVertexIndex());

        mIndexOutdated = false;
    }

    mView.draw();

    if (mLeftBtnMousePressed)
    {
        drawSquare();
    }

    // end the current frame
    window->display();

}

void processEvent(sf::Event event, sf::RenderWindow &window)
{
    // "close requested" event: we close the window
    if (event.type == sf::Event::Closed)
    {
        mMaze.save(FILENAME_OUTPUT);

        // end the program
        mRunning = false;
    }

    if (event.type == sf::Event::Resized)
    {
        // adjust the viewport when the window is resized
        resize(event.size.width, event.size.height);
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
    std::vector<VertexAttribs> squareVertex;
    VertexAttribs v;

    v.setColor(1, 0, 0);

    v.setXYZW(mLeftBtnStartX, -mLeftBtnStartY, 0, 1);
    squareVertex.push_back(v);

    v.setXYZW(mLeftBtnCurrentX, -mLeftBtnStartY, 0, 1);
    squareVertex.push_back(v);

    v.setXYZW(mLeftBtnCurrentX, -mLeftBtnCurrentY, 0, 1);
    squareVertex.push_back(v);

    v.setXYZW(mLeftBtnStartX, -mLeftBtnCurrentY, 0, 1);
    squareVertex.push_back(v);

    mViewSquare.setVertexData(squareVertex);

    mViewSquare.draw();

}

void updateMaze()
{
    bool isUpdated = mMaze.update(
        min(mLeftBtnStartX, mLeftBtnCurrentX),
        min(mLeftBtnStartY, mLeftBtnCurrentY),
        max(mLeftBtnStartX, mLeftBtnCurrentX),
        max(mLeftBtnStartY, mLeftBtnCurrentY)
        );

    if (isUpdated)
    {
        mIndexOutdated = true;
    }
}

void resize(int w, int h)
{
    printf("Window resized: %d, %d\n", w, h);

    //delegate to our view class.
    mView.resize(w, h);
    mViewSquare.resize(w, h);

    mMaze.resize(w, h);

    mDataOutdated = true;

    //sets the viewport to cover the entire area of the resized window
    //glViewport(leftx,topy,width,height)
    glViewport(0, 0, w, h);
}

void initSquare() {
    mViewSquare.initialize();

    std::vector<GLuint> squareIndex;

    for (int i = 0; i < 4; i++)
    {
        squareIndex.push_back(i);
        squareIndex.push_back((i + 1) % 4);
    }

    mViewSquare.setVertexIndex(squareIndex);
}

void init(sf::RenderWindow *window)
{
    /* Some configurations */
    resize(INIT_WIDTH, INIT_HEIGHT);

    window->setFramerateLimit(30);

    glClearColor(1, 1, 1, 0); // set clear code to white

    mView.initialize();

    initSquare();
}