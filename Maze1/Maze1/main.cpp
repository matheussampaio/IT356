/*
 * This is the main file. It sets up the window using sfml and initializes OpenGL extensions using glew
 *
 * We use sfml, an open-source library. SFML has very basic support for guis. There is SFGUI but I have not used it yet. 
 * SFML is by no means a full-fledged powerful UI framework, but it is very simple to use, and is cross-platform. But one of the things going for it is speed.
 * Feel free to use and modify this file for every project.
 *
 * SFML was written for C++ and opengl was written for C
 * In order to initialize OpenGL extensions to use OpenGL 4, we use glew. This simplifies setting it up for every program.
 **/


#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include "View.h"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

/* function prototypes */
void init();
void resize(int w,int h);
void display(sf::RenderWindow *window);
void processEvent(sf::Event event,sf::RenderWindow& window);
void drawText(sf::RenderWindow& window,string text,int x,int y);

View v; //an object to our View class that encapsulates everything that we do.
sf::Font font;
sf::Clock sfclock;
int frames;
double frame_rate;

int main(int argc, char *argv[])
{
	frames = 0;
	frame_rate = 0;
    // Request a 32-bits depth buffer when creating the window
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 32;
	contextSettings.majorVersion = 4;
	contextSettings.minorVersion = 0;

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Hello World", sf::Style::Default, contextSettings);
	resize(800,600);
  //  window.setVerticalSyncEnabled(true);

    // Make it the active window for OpenGL calls
 	window.setActive();   

    //glew currently has a bug that causes problems with 4.0. Setting this glew flag is the temporary solution for it
    glewExperimental = GL_TRUE;

    //initialize glew which initializes all the OpenGL extensions.
    if (glewInit()!=GLEW_OK)
    {
        cerr << "Unable to initialize GLEW...exiting" << endl;
        return EXIT_FAILURE;
    }

    //initialize stuff. This will likely change with every program.
    init();

	
// Start game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
			processEvent(event,window);

        }

		if (window.isOpen())
			display(&window);
    }

    

    return EXIT_SUCCESS;
}

/*
 * This will be the function that processes any events
*/

void processEvent(sf::Event event,sf::RenderWindow& window)
{
	// Close window : exit
	if (event.type == sf::Event::Closed)
		window.close();

	// Escape key : exit
	if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
		window.close();

	// Adjust the viewport when the window is resized
	if (event.type == sf::Event::Resized)
		resize(event.size.width, event.size.height);

	if (event.type == sf::Event::MouseWheelMoved)
	{
		cout << "Mouse wheel moved" << endl;
	}
}

/*
 * This function helps us to draw text over a part of the window
 */

void drawText(sf::RenderWindow *window,string text,int x,int y)
{
	// Create some text to draw on top of our OpenGL object
  
    sf::Text textobj(text, font);

	textobj.setCharacterSize(18);
    textobj.setColor(sf::Color(255, 255, 255, 255));
    textobj.setPosition((float)x,(float)y);
	

	window->pushGLStates();
	window->resetGLStates();
	window->draw(textobj);
    window->popGLStates();
}


/*
 * This will be our display function. Whenever glut needs to redraw the window, it will call this function.
 * The name of this function (display) is of no consequence: you can name it whatever you want, so long as the constraints below are obeyed.
 * Thus all your rendering code should be in this function, or should be called from this function.
 *
*/

void display(sf::RenderWindow *window)
{
	if (frames==0)
		sfclock.restart();
	
	// Draw using SFML
	window->pushGLStates();
	window->resetGLStates();
	//insert SFML drawing code here (any part you are using that does not involve opengl code)
	window->popGLStates();

	//set up the background color of the window. This does NOT clear the window. Right now it is (0,0,0) which is black
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //this command actually clears the window.
	glEnable(GL_DEPTH_TEST);
	v.draw(); //simply delegate to our view class that has all the data and does all the rendering

	if (frames>500)
	{
		sf::Time t = sfclock.getElapsedTime();
		frame_rate = frames/t.asSeconds();
		frames = 0;
	}
	else
	{
		frames++;
	}
	stringstream str;

	str << "Frame rate " << frame_rate;
	// Draw some text on top of our OpenGL object
	drawText(window,str.str(),window->getSize().x-200,50);
    

	// Finally, display the rendered frame on screen
	window->display();
//	cout << "Rendering" << endl;
}

/*
 * This function will be called by glut whenever the window resizes. This happens initially when it creates the window, and later if the user manually resizes the window or maximizes it.
 * The name of this function (resize) is of no consequence: you can name it whatever you want, so long as the constraints below are obeyed.
 * This function must take two integers as parameters, the width and height of the resized window in that order
 * This function must return void.
 **/
void resize(int w,int h)
{
    //delegate to our view class.
    v.resize(w,h);

    //sets the viewport to cover the entire area of the resized window
    //glViewport(leftx,topy,width,height)
    glViewport(0,0,w,h);
}

void init()
{
    int major,minor;
    v.getOpenGLVersion(&major,&minor);

    cout <<"Opengl version supported : "<<major<<"."<<minor<<endl;
    v.getGLSLVersion(&major,&minor);
    cout << "GLSL version supported : "<<major<<"."<<minor << endl;
    //delegate to our view class to do all the initializing
    v.initialize();

	if (!font.loadFromFile("resources/sansation.ttf"))
		return;

	
}
