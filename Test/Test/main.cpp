#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include "Maze.cpp"

using namespace std;

void display(sf::RenderWindow *window);

Maze maze;

int main(int argc, char *argv[])
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 800), "My window");

    // create the tilemap from the level definition

    if (!maze.init("maze-20x20.txt", 800, 800)) 
    {
        cerr << "map failed." << endl;
        return -1;
    }
    else {
        cout << "map loaded." << endl;
    }
    

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (window.isOpen()) {
            display(&window);
        }
    }

    return 0;
}

void display(sf::RenderWindow *window) {

    // clear the window with black color
    window->clear(sf::Color::White);

    window->draw(maze);

    // end the current frame
    window->display();
}