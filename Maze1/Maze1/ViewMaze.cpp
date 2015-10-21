#include "ViewMaze.h"
#include <GL/glew.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

//glm headers to access various matrix producing functions, like ortho below in resize
#include <glm/gtc/matrix_transform.hpp>
//the glm header required to convert glm objects into normal float/int pointers expected by OpenGL
//see value_ptr function below for an example
#include <glm/gtc/type_ptr.hpp>

ViewMaze::ViewMaze()
    :NumVertices(6)
{
}

void ViewMaze::resize(int w, int h)
{
    WINDOW_WIDTH = w;
    WINDOW_HEIGHT = h;

    /*
    *This program uses orthographic projection. The corresponding matrix for this projection is provided by the glm function below.
    */
    proj = glm::ortho(0.0f, WINDOW_WIDTH * 1.0f, WINDOW_HEIGHT * -1.0f, 0.0f); 
}

void ViewMaze::initialize()
{
    //populate our shader information. The two files below are present in this project.
    ShaderInfo shaders[] =
    {
        { GL_VERTEX_SHADER, "default.vert" },
        { GL_FRAGMENT_SHADER, "default.frag" },
        { GL_NONE, "" }
    };

    //call helper function, get the program shader ID if everything went ok.
    program = createShaders(shaders);


    //use the above program. After this statement, any rendering will use this above program
    //passing 0 to the function below disables using any shaders
    glUseProgram(program);


    //assuming the program above is compiled and linked correctly, get IDs for all the input variables
    //that the shader expects our program to provide.
    //think of these IDs as references to those shader variables

    //the second parameter of glGetUniformLocation is a string that is the name of an actual variable in the shader program
    //this variable may exist in any of the shaders that are linked in this program.
    projectionLocation = glGetUniformLocation(program, "projection");
    modelViewLocation = glGetUniformLocation(program, "modelview");

    vPositionLocation = glGetAttribLocation(program, "vPosition");
    vColorLocation = glGetAttribLocation(program, "vColor");

    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    glGenBuffers(NumBuffers, vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[ArrayBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertexAttribs)*mVertexData.size(), NULL, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[IndexBuffer]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*mVertexIndex.size(), NULL, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[ArrayBuffer]);

    glVertexAttribPointer(vPositionLocation, 4, GL_FLOAT, GL_FALSE, sizeof(MyVertexAttribs), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vPositionLocation);

    glVertexAttribPointer(vColorLocation, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertexAttribs), BUFFER_OFFSET(4 * sizeof(GLfloat)));
    glEnableVertexAttribArray(vColorLocation);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

/*
*Function called from display in main. Should contain or call all the code to draw one screen from scratch
*entirely.
*/
void ViewMaze::draw()
{
    glUseProgram(program);

    modelView = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(proj));
    glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, glm::value_ptr(modelView));

    glBindVertexArray(vao);

    glDrawElements(GL_LINES, mVertexIndex.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

    glBindVertexArray(0);

    glFlush();

    glUseProgram(0);

}

/*
*This is a helper function that will take shaders info as a parameter, compiles them and links them
*into a shader program.
*
*This function is standard and should not change from one program to the next.
*/
GLuint ViewMaze::createShaders(ShaderInfo *shaders)
{
    ifstream file;
    GLuint shaderProgram;
    GLint linked;

    ShaderInfo *entries = shaders;

    shaderProgram = glCreateProgram();


    while (entries->type != GL_NONE)
    {
        file.open(entries->filename.c_str());
        GLint compiled;


        if (!file.is_open())
            return false;

        string source, line;


        getline(file, line);
        while (!file.eof())
        {
            source = source + "\n" + line;
            getline(file, line);
        }
        file.close();


        const char *codev = source.c_str();


        entries->shader = glCreateShader(entries->type);
        glShaderSource(entries->shader, 1, &codev, NULL);
        glCompileShader(entries->shader);
        glGetShaderiv(entries->shader, GL_COMPILE_STATUS, &compiled);

        if (!compiled)
        {
            printShaderInfoLog(entries->shader);
            for (ShaderInfo *processed = shaders; processed->type != GL_NONE; processed++)
            {
                glDeleteShader(processed->shader);
                processed->shader = 0;
            }
            return 0;
        }
        glAttachShader(shaderProgram, entries->shader);
        entries++;
    }

    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked);

    if (!linked)
    {
        printShaderInfoLog(entries->shader);
        for (ShaderInfo *processed = shaders; processed->type != GL_NONE; processed++)
        {
            glDeleteShader(processed->shader);
            processed->shader = 0;
        }
        return 0;
    }


    return shaderProgram;
}

void ViewMaze::printShaderInfoLog(GLuint shader)
{
    int infologLen = 0;
    int charsWritten = 0;
    GLubyte *infoLog;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);
    //	printOpenGLError();
    if (infologLen > 0)
    {
        infoLog = (GLubyte *)malloc(infologLen);
        if (infoLog != NULL)
        {
            glGetShaderInfoLog(shader, infologLen, &charsWritten, (char *)infoLog);
            printf("InfoLog: %s\n\n", infoLog);
            free(infoLog);
        }

    }
    //	printOpenGLError();
}

void ViewMaze::getOpenGLVersion(int *major, int *minor)
{
    const char *verstr = (const char *)glGetString(GL_VERSION);
    if ((verstr == NULL) || (sscanf_s(verstr, "%d.%d", major, minor) != 2))
    {
        *major = *minor = 0;
    }
}

void ViewMaze::getGLSLVersion(int *major, int *minor)
{
    int gl_major, gl_minor;

    getOpenGLVersion(&gl_major, &gl_minor);
    *major = *minor = 0;

    if (gl_major == 1)
    {
        /* GL v1.x can only provide GLSL v1.00 as an extension */
        const char *extstr = (const char *)glGetString(GL_EXTENSIONS);
        if ((extstr != NULL) && (strstr(extstr, "GL_ARB_shading_language_100") != NULL))
        {
            *major = 1;
            *minor = 0;
        }
    }
    else if (gl_major >= 2)
    {
        /* GL v2.0 and greater must parse the version string */
        const char *verstr = (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION);
        if ((verstr == NULL) || (sscanf_s(verstr, "%d.%d", major, minor) != 2))
        {
            *major = 0;
            *minor = 0;
        }
    }
}

void ViewMaze::setVertexIndex(vector<GLuint> vertexIndex) {
    std::cout << "Updating vertex index" << std::endl;

    mVertexIndex = vertexIndex;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[IndexBuffer]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mVertexIndex.size(), &mVertexIndex[0], GL_DYNAMIC_DRAW);
}

void ViewMaze::setVertexData(vector<MyVertexAttribs> vertexData) {
    std::cout << "Updating vertex data" << std::endl;

    mVertexData = vertexData;

    glBindBuffer(GL_ARRAY_BUFFER, vbo[ArrayBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertexAttribs) * mVertexData.size(), &mVertexData[0], GL_DYNAMIC_DRAW);
}