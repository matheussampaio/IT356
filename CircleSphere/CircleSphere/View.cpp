#include "View.h"
#include <GL/glew.h>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;
#include "utils/TriangleMesh.h"
//glm headers to access various matrix producing functions, like ortho below in resize
#include <glm/gtc/matrix_transform.hpp>
//the glm header required to convert glm objects into normal float/int pointers expected by OpenGL
//see value_ptr function below for an example
#include <glm/gtc/type_ptr.hpp>

View::View()
{
	meshObject = NULL;
	cam_pos[1] = 150;
	theta = 0;
}

View::~View()
{
	if (meshObject!=NULL)
		delete meshObject;
}

void View::initObjects()
{
	TriangleMesh tmesh;
	

	int SLICES = 20;
	int STACKS = 10;
	vector<VertexAttribs> verts;
	vector<unsigned int> triangles;
	double PI = 3.14159;
	VertexAttribs v;

	v.position = glm::vec4(0,0,0,1);
	verts.push_back(v);

	for (int i=0;i<SLICES;i++)
	{
		
		double theta = 2*PI*i/(SLICES-1);

		v.position = glm::vec4(cos(theta),sin(theta),0,1);

		verts.push_back(v);
	}

/*	//for GL_TRIANGLES
	for (int i=0;i<SLICES;i++)
	{
		triangles.push_back(0);
		triangles.push_back(1+i);
		triangles.push_back(1+(i+1)%SLICES);
	}
	*/

	//GL_TRIANGLE_FAN
	for (int i=0;i<SLICES;i++)
	{
		triangles.push_back(i);
	}
	triangles.push_back(1);
	

/*	//GL_LINES
	for (int i=0;i<SLICES;i++)
	{
		triangles.push_back(1+i);
		triangles.push_back(1+(i+1)%SLICES);
	}
	*/

/*	//GL_LINE_STRIP
	for (int i=1;i<SLICES;i++)
	{		
		triangles.push_back(i);
	}
	triangles.push_back(1);
	*/

/*	//sphere

	for (int i=0;i<STACKS;i++)
	{
		double phi;

		phi = -0.5*PI + PI*i/(STACKS-1);
		for (int j=0;j<SLICES;j++)
		{
			double theta;
			
			theta = 2*PI*j/(SLICES);

			v.position = glm::vec4(cos(phi)*cos(theta),sin(phi),cos(phi)*sin(theta),1);

			verts.push_back(v);
		}
	}

	//GL_TRIANGLES
	for (int i=0;i<STACKS-1;i++)
	{
		for (int j=0;j<SLICES;j++)
		{
			triangles.push_back(i*SLICES+j);
			triangles.push_back(i*SLICES+(j+1)%SLICES);
			triangles.push_back(((i+1)%STACKS)*SLICES+(j+1)%SLICES);

			triangles.push_back(i*SLICES+j);
			triangles.push_back(((i+1)%STACKS)*SLICES+(j+1)%SLICES);
			triangles.push_back(((i+1)%STACKS)*SLICES+j);
		}
	}
*/
	/*//GL_POINTS
	for (int i=0;i<STACKS*SLICES;i++)
	{
		triangles.push_back(i);
	}
	*/



	
	tmesh.initVertexData(verts);
	tmesh.initTriangles(triangles);

	meshObject = new graphics::Object(tmesh);
	

	graphics::Material mat;

	mat.setAmbient(1,1,1);
	mat.setDiffuse(1,1,1);
	mat.setSpecular(1,1,1);

	meshObject->setMaterial(mat);
	
	//set the size of the object. By default each mesh is in a cube of side 1 centered at the origin
	meshObject->setTransform(glm::scale(glm::mat4(1.0),glm::vec3(200,200,200)));

	//place the camera accordingly
	glm::vec4 minBounds = meshObject->getMinimumWorldBounds();
	glm::vec4 maxBounds = meshObject->getMaximumWorldBounds();
	float dist = glm::distance(maxBounds,minBounds);

	camera.init(glm::vec3(dist,dist,2*dist),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));

}


void View::resize(int w, int h)
{
    WINDOW_WIDTH = w;
    WINDOW_HEIGHT = h;

    /*
     *This program uses orthographic projection. The corresponding matrix for this projection is provided by the glm function below.
     */

//    proj = glm::perspective(120.0f,(float)WINDOW_WIDTH/WINDOW_HEIGHT,0.1f,10000.0f);
	   proj = glm::ortho((float)-1*WINDOW_WIDTH/2,(float)1*WINDOW_WIDTH/2,(float)-1*WINDOW_HEIGHT/2,(float)1*WINDOW_HEIGHT/2,0.1f,10000.0f);

}

void View::initialize()
{
    //populate our shader information. The two files below are present in this project.
    ShaderInfo shaders[] =
    {
        {GL_VERTEX_SHADER,"default.vert"},
        {GL_FRAGMENT_SHADER,"default.frag"},
        {GL_NONE,""}
    };

	
	

	
    //call helper function, get the program shader ID if everything went ok.
    program = createShaders(shaders);

	if (program<=0)
		exit(0);

	initObjects();

    //use the above program. After this statement, any rendering will use this above program
    //passing 0 to the function below disables using any shaders
    glUseProgram(program);


    //assuming the program above is compiled and linked correctly, get IDs for all the input variables
    //that the shader expects our program to provide.
    //think of these IDs as references to those shader variables

    //the second parameter of glGetUniformLocation is a string that is the name of an actual variable in the shader program
    //this variable may exist in any of the shaders that are linked in this program.
    projectionLocation = glGetUniformLocation(program,"projection");
    modelViewLocation = glGetUniformLocation(program,"modelview");
    colorLocation = glGetUniformLocation(program,"vColor");
    
	

	glUseProgram(0);
}

/*
 *Function called from display in main. Should contain or call all the code to draw one screen from scratch
 *entirely.
 */

void View::draw()
{
//	theta += 0.1;
	glUseProgram(program);
	
	glEnable(GL_LINE_SMOOTH);// or GL_POLYGON_SMOOTH 
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); ;
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); //GL_FASTEST,GL_DONT_CARE 

    /*
     *We would like to scale up the triangles. We use glm::scale to get the scale matrix.
     *We pass this along as the modelview matrix to the shader, which multiplies each incoming vertex's
     *position with this matrix
     */

	modelView = glm::lookAt(glm::vec3(0,0,10),glm::vec3(0,0,0),glm::vec3(0,1,0)) * meshObject->getTransform();

	float cam_radius = 20;


//	modelView = glm::lookAt(glm::vec3(cam_radius*cos(glm::radians(theta)),cam_radius,cam_radius*sin(glm::radians(theta))),glm::vec3(0,0,0),glm::vec3(0,1,0)) * meshObject->getTransform();
  //  modelView = camera.getWorldToCameraTransform() * meshObject->getTransform();


	// modelView = glm::lookAt(glm::vec3(0,cam_radius,0),glm::vec3(0,0,0),glm::vec3(1,0,0)) * meshObject->getTransform();

    /*
     *Supply the shader with all the matrices it expects.
     *We already have all the pointers for those matrices from "init" above
     *The value_ptr creates a copy-by-value of the glm::mat4 into an array of floats as OpenGL expects
     */

    glUniformMatrix4fv(projectionLocation,1,GL_FALSE,glm::value_ptr(proj));

    glUniformMatrix4fv(modelViewLocation,1,GL_FALSE,glm::value_ptr(modelView));

	
	
	//material properties
    glUniform3fv(colorLocation,1,glm::value_ptr(meshObject->getMaterial().getAmbient()));
    
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); //draw in line mode. Every triangle is drawn as 3 lines
	meshObject->draw(GL_TRIANGLE_FAN);


    /*
     *OpenGL batch-processes all its OpenGL commands.
     *The next command asks OpenGL to "empty" its batch of issued commands, i.e. draw
     *
     *This a non-blocking function. That is, it will signal OpenGL to draw, but won't wait for it to
     *finish drawing.
     *
     *If you would like OpenGL to start drawing and wait until it is done, call glFinish() instead.
     */
    glFlush();

	glUseProgram(0);
	
}

void View::advanceCamera()
{
	camera.advance();
}

void View::retractCamera()
{
	camera.retract();
}
void View::rotateLeftCamera()
{
	camera.rotateLeft();
}
void View::rotateRightCamera()
{
	camera.rotateRight();
}
void View::rotateCWCamera()
{
	camera.rotateCW();
}
void View::rotateCCWCamera()	
{
	camera.rotateCCW();
}

void View::rotateUpCamera()
{
	camera.rotateUp();
}

void View::rotateDownCamera()
{
	camera.rotateDown();
}

void View::accelerateCamera()
{
	camera.increaseSpeed();
}

void View::decelerateCamera()
{
	camera.decreaseSpeed();
}





/*
 *This is a helper function that will take shaders info as a parameter, compiles them and links them
 *into a shader program.
 *
 *This function is standard and should not change from one program to the next.
 */
GLuint View::createShaders(ShaderInfo *shaders)
{
    ifstream file;
    GLuint shaderProgram;
    GLint linked;

    ShaderInfo *entries = shaders;

    shaderProgram = glCreateProgram();


    while (entries->type !=GL_NONE)
    {
        file.open(entries->filename.c_str());
        GLint compiled;


        if (!file.is_open())
            return false;

        string source,line;


        getline(file,line);
        while (!file.eof())
        {
            source = source + "\n" + line;
            getline(file,line);
        }
        file.close();


        const char *codev = source.c_str();


        entries->shader = glCreateShader(entries->type);
        glShaderSource(entries->shader,1,&codev,NULL);
        glCompileShader(entries->shader);
        glGetShaderiv(entries->shader,GL_COMPILE_STATUS,&compiled);

        if (!compiled)
        {
            printShaderInfoLog(entries->shader);
            for (ShaderInfo *processed = shaders;processed->type!=GL_NONE;processed++)
            {
                glDeleteShader(processed->shader);
                processed->shader = 0;
            }
            return 0;
        }
        glAttachShader( shaderProgram, entries->shader );
        entries++;
    }

    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram,GL_LINK_STATUS,&linked);

    if (!linked)
    {
        printShaderInfoLog(entries->shader);
        for (ShaderInfo *processed = shaders;processed->type!=GL_NONE;processed++)
        {
            glDeleteShader(processed->shader);
            processed->shader = 0;
        }
        return 0;
    }


    return shaderProgram;
}

void View::printShaderInfoLog(GLuint shader)
{
    int infologLen = 0;
    int charsWritten = 0;
    GLubyte *infoLog;

    glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&infologLen);
    //	printOpenGLError();
    if (infologLen>0)
    {
        infoLog = (GLubyte *)malloc(infologLen);
        if (infoLog != NULL)
        {
            glGetShaderInfoLog(shader,infologLen,&charsWritten,(char *)infoLog);
            printf("InfoLog: %s\n\n",infoLog);
            free(infoLog);
        }

    }
//	printOpenGLError();
}

void View::getOpenGLVersion(int *major,int *minor)
{
    const char *verstr = (const char *)glGetString(GL_VERSION);
    if ((verstr == NULL) || (sscanf_s(verstr,"%d.%d",major,minor)!=2))
    {
        *major = *minor = 0;
    }
}

void View::getGLSLVersion(int *major,int *minor)
{
    int gl_major,gl_minor;

    getOpenGLVersion(&gl_major,&gl_minor);
    *major = *minor = 0;

    if (gl_major==1)
    {
        /* GL v1.x can only provide GLSL v1.00 as an extension */
        const char *extstr = (const char *)glGetString(GL_EXTENSIONS);
        if ((extstr!=NULL) && (strstr(extstr,"GL_ARB_shading_language_100")!=NULL))
        {
            *major = 1;
            *minor = 0;
        }
    }
    else if (gl_major>=2)
    {
        /* GL v2.0 and greater must parse the version string */
        const char *verstr = (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION);
        if ((verstr==NULL) || (sscanf_s(verstr,"%d.%d",major,minor) !=2))
        {
            *major = 0;
            *minor = 0;
        }
    }
}
