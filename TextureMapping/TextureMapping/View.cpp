#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#include "View.h"
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <cstdlib>
#include <fstream>
#include <vector>
using namespace std;
#include "utils/OBJImporter.h"


//glm headers to access various matrix producing functions, like ortho below in resize
#include <glm/gtc/matrix_transform.hpp>
//the glm header required to convert glm objects into normal float/int pointers expected by OpenGL
//see value_ptr function below for an example
#include <glm/gtc/type_ptr.hpp>


View::View()
{
   time = 0;
}

View::~View()
{
    for (int i=0;i<objectsList.size();i++)
    {
        delete objectsList[i];
    }

    objectsList.clear();
}

void View::resize(int w, int h)
{
	float range = 20;
    //record the new dimensions
    WINDOW_WIDTH = w;
    WINDOW_HEIGHT = h;

    /*
     * This program uses orthographic projection. The corresponding matrix for this projection is provided by the glm function below.
     *The last two parameters are for the near and far planes.
     *
     *Very important: the last two parameters specify the position of the near and far planes with respect
     *to the eye, in the direction of gaze. Thus positive values are in front of the camera, and negative
     *values are in the back!
     **/
   
    while (!proj.empty())
        proj.pop();

	//proj.push(glm::ortho(-0.5f*range,0.5f*range,-0.5f*range*WINDOW_HEIGHT/WINDOW_WIDTH,0.5f*range*WINDOW_HEIGHT/WINDOW_WIDTH,0.1f,10000.0f));
    proj.push(glm::perspective(120.0f,(float)WINDOW_WIDTH/WINDOW_HEIGHT,0.1f,10000.0f));
}

void View::initialize()
{
    //populate our shader information. The two files below are present in this project.
    ShaderInfo shaders[] =
    {
        {GL_VERTEX_SHADER,"phong-multiple.vert"},
        {GL_FRAGMENT_SHADER,"phong-multiple.frag"},
        {GL_NONE,""}
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

    projectionLocation = glGetUniformLocation(program,"projection");
    modelviewLocation = glGetUniformLocation(program,"modelview");
    normalMatrixLocation = glGetUniformLocation(program,"normalmatrix");
	texturematrixLocation = glGetUniformLocation(program,"texturematrix");
    numLightsLocation = glGetUniformLocation(program,"numLights");


    mat_ambientLocation = glGetUniformLocation(program,"material.ambient");
    mat_diffuseLocation = glGetUniformLocation(program,"material.diffuse");
    mat_specularLocation = glGetUniformLocation(program,"material.specular");
    mat_shininessLocation = glGetUniformLocation(program,"material.shininess");
	textureLocation = glGetUniformLocation(program,"image");

	graphics::Light l;

	
    l.setAmbient(0.8f,0.8f,0.8f);
    l.setDiffuse(0.8f,0.8f,0.8f);
    l.setSpecular(0.8f,0.8f,0.8f);
	l.setPosition(300,300,300);
	light.push_back(l);

    for (int i=0;i<light.size();i++)
    {
        stringstream name;

        name << "light[" << i << "].ambient";

        lightLocation[i].ambientLocation = glGetUniformLocation(program,name.str().c_str());

        name.clear();//clear any bits set
        name.str(std::string());

        name << "light[" << i << "].diffuse";

        lightLocation[i].diffuseLocation = glGetUniformLocation(program,name.str().c_str());

        name.clear();//clear any bits set
        name.str(std::string());

        name << "light[" << i << "].specular";

        lightLocation[i].specularLocation = glGetUniformLocation(program,name.str().c_str());

        name.clear();//clear any bits set
        name.str(std::string());

        name << "light[" << i << "].position";

        lightLocation[i].positionLocation = glGetUniformLocation(program,name.str().c_str());

        name.clear();//clear any bits set
        name.str(std::string());

    }

	


	graphics::Object * o;
	TriangleMesh m;
	graphics::Material mat;
	
	
	o = new graphics::Object();
	OBJImporter::importFile(m,string("models/sphere"),false);
	o->initTriangleMesh(m);

    mat.setAmbient(0.2f,0.2f,0.2f);
    mat.setDiffuse(0.8f,0.8f,0.8f);
    mat.setSpecular(0.8f,0.8f,0.8f);
    mat.setShininess(100.0f);
	o->setMaterial(mat);

	glm::vec4 center = (o->getMinimumWorldBounds() + o->getMaximumWorldBounds())*0.5f;

	trackball.initCenter(center.xyz());
	trackball.initRadius(1500);

	//o->setTextureTransform(glm::scale(glm::mat4(1.0),glm::vec3(5,5,5)));
	
	o->setTransform(glm::scale(glm::mat4(1.0),glm::vec3(50,50,50)));
	objectsList.push_back(o);
	
	


	for (int i=0;i<objectsList.size();i++)
	{
		animation_transform.push_back(glm::mat4(1.0));
	}

	/* texture initialization */

//    textureID = loadTexture("earthmap.ppm");

    sf::Image im;
	
	im.loadFromFile("earthmap.png");

	//origin of the image is top left, all our texture coordinates are with respect to bottom left, so flip image vertically
	im.flipVertically();

    
    glGenTextures(1,&textureID); //get a unique texture ID
    glBindTexture(GL_TEXTURE_2D,textureID);// bind this texture as "current". All texture commands henceforth will apply to this ID

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, im.getSize().x,im.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, im.getPixelsPtr());
	
	//glGenerateMipmap(GL_TEXTURE_2D);
	

	glUseProgram(0);

}

void View::draw()
{
    /*
     *The modelview matrix for the View class is going to store the world-to-view transformation
     *This effectively is the transformation that changes when the camera parameters chang
     *This matrix is provided by glm::lookAt
     */
	time +=0.01;
	//animate();

	glUseProgram(program);
    while (!modelview.empty())
        modelview.pop();

    modelview.push(glm::mat4(1.0));
    
	modelview.top() = modelview.top() * glm::lookAt(glm::vec3(0,0,200),glm::vec3(0,0,0),glm::vec3(0,1,0));
	
	modelview.top() = modelview.top() * trackball.getTransform();
    glUniformMatrix4fv(projectionLocation,1,GL_FALSE,glm::value_ptr(proj.top()));

	glUniform1i(numLightsLocation,1);

	

    //light properties
    for (int i=0;i<light.size();i++)
    {
        glUniform3fv(lightLocation[i].ambientLocation,1,glm::value_ptr(light[i].getAmbient()));
        glUniform3fv(lightLocation[i].diffuseLocation,1,glm::value_ptr(light[i].getDiffuse()));
        glUniform3fv(lightLocation[i].specularLocation,1,glm::value_ptr(light[i].getSpecular()));
        glUniform4fv(lightLocation[i].positionLocation,1,glm::value_ptr(light[i].getPosition()));
    }

	glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);


    glBindTexture(GL_TEXTURE_2D,textureID);
    glUniform1i(textureLocation,0); //bind GL_TEXTURE0 to sampler2D (whatever is bound to GL_TEXTURE0)

    /*
     *Instead of directly supplying the modelview matrix to the shader here, we pass it to the objects
     *This is because the object's transform will be multiplied to it before it is sent to the shader
     *for vertices of that object only.
     *
     *Since every object is in control of its own color, we also pass it the ID of the color
     *in the activated shader program.
     *
     *This is so that the objects can supply some of their attributes without having any direct control
     *of the shader itself.
     */

    for (int i=0;i<objectsList.size();i++)
    {
        glm::mat4 transform = objectsList[i]->getTransform();
        
        //The total transformation is whatever was passed to it, with its own transformation
        glUniformMatrix4fv(modelviewLocation,1,GL_FALSE,glm::value_ptr(modelview.top() * animation_transform[i]* transform));
		glUniformMatrix4fv(texturematrixLocation,1,GL_FALSE,glm::value_ptr(objectsList[i]->getTextureTransform()));
		glUniformMatrix4fv(normalMatrixLocation,1,GL_FALSE,glm::value_ptr(glm::transpose(glm::inverse(modelview.top()))));

        //material properties
        glUniform3fv(mat_ambientLocation,1,glm::value_ptr(objectsList[i]->getMaterial().getAmbient()));
        glUniform3fv(mat_diffuseLocation,1,glm::value_ptr(objectsList[i]->getMaterial().getDiffuse()));
        glUniform3fv(mat_specularLocation,1,glm::value_ptr(objectsList[i]->getMaterial().getSpecular()));
        glUniform1f(mat_shininessLocation,objectsList[i]->getMaterial().getShininess());
        objectsList[i]->draw(GL_TRIANGLES);
    }
	glFinish();
    modelview.pop();
	glUseProgram(0);
}

void View::animate()
{
	


}

void View::mousepress(int x, int y)
{
	prev_mouse = glm::vec2(x,y);
}

void View::mousemove(int x, int y)
{
   int dx,dy;

    dx = x - prev_mouse.x;
    dy = (y) - prev_mouse.y;

	if ((dx==0) && (dy==0))
		return;

	trackball.change(dx,dy);
	prev_mouse.x = x;
	prev_mouse.y = y;

	/*
    //(-dy,dx) gives the axis of rotation

    //the angle of rotation is calculated in radians by assuming that the radius of the trackball is 300
    float angle = sqrt((float)dx*dx+dy*dy)/300;
   
    prev_mouse = glm::vec2(x,y);
	
    trackballTransform = glm::rotate(glm::mat4(1.0),angle,glm::vec3(-dy,dx,0)) * trackballTransform;
	*/
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
