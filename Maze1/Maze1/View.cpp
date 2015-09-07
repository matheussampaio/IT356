#include "View.h"
#include <GL/glew.h>
#include <cstdlib>
#include <fstream>
#include <vector>
using namespace std;

//glm headers to access various matrix producing functions, like ortho below in resize
#include <glm/gtc/matrix_transform.hpp>
//the glm header required to convert glm objects into normal float/int pointers expected by OpenGL
//see value_ptr function below for an example
#include <glm/gtc/type_ptr.hpp>

View::View()
	:NumVertices(6)
{
}

void View::resize(int w, int h)
{
	WINDOW_WIDTH = w;
	WINDOW_HEIGHT = h;

	/*
	 *This program uses orthographic projection. The corresponding matrix for this projection is provided by the glm function below.
	 */

	proj = glm::ortho(-WINDOW_WIDTH / 2.0f, WINDOW_WIDTH / 2.0f, -WINDOW_HEIGHT / 2.0f, WINDOW_HEIGHT / 2.0f);

}

void View::initialize()
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


	/*
	 * Now we setup everything to draw.
	 * This particular program just draws two triangles on the screen.
	 * To draw a triangle, we need three vertices.
	 * Each vertex must have a position and a color for us to see it.
	 **/

	/*
	 * the positions of the 6 vertices of the 3 triangles. This is not the most efficient way of
	 * specifying positions.
	 *
	 * This example is drawing only 2D triangles, which are triangles with z=0 for all vertices.
	 * The last number of each vertex should be a 1 making it a point and not a direction
	 **/

	GLfloat vertices[][4] =
	{
		{ -0.9f, -0.90f, 0, 1.0f },
		{ 0.85f, -0.90f, 0, 1.0f },
		{ -0.9f, 0.85f, 0, 1.0f },
		{ 0.9f, -0.85f, 0, 1.0f },
		{ 0.9f, 0.90f, 0, 1.0f },
		{ -0.85f, 0.90f, 0, 1.0f }
	};

	/*
	 * The color for each of the above vertices. Each color has three numbers: red, green, blue.
	 */
	GLfloat colors[][3] =
	{
		{ 1, 0, 0 }, //red
		{ 0, 1, 0 }, //green
		{ 0, 0, 1 }, //blue
		{ 1, 0, 0 }, //red
		{ 0, 1, 0 }, //green
		{ 0, 0, 1 }  //blue
	};

	/*
	 *Next we must provide information about how the above vertices are assembled into triangles
	 *In other words, which 3 vertices are used by each triangle.
	 *Thus we specify a triangle in terms of 3 indices. These indices point to which vertex positions
	 *and color to use to assemble that triangle
	 *
	 *Therefore, we need a total of 6 integer indices, 3 for each Triangles
	 */
	vector<GLuint> indices;

	//the first triangle is composed of vertex 0,1,2 in the above arrays
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	//the second triangle is composed of vertex 3,4,5 in the above arrays
	indices.push_back(3);
	indices.push_back(4);
	indices.push_back(5);


	/*
	 * It is often convenient to have all the data for a vertex together, instead of having
	 * a separate array for each attribute of the vertex as we have above.
	 * Therefore we now create an array of VertexAttribs objects.
	 * Each object will encapsulate the position and color of a vertex
	 */
	vector<VertexAttribs> vertexdata;

	for (int i = 0; i < 6; i++)
	{
		VertexAttribs v;

		vertexdata.push_back(v);
		for (int j = 0; j < 3; j++)
		{
			vertexdata[i].color[j] = colors[i][j];
		}

		for (int j = 0; j < 4; j++)
		{
			vertexdata[i].position[j] = vertices[i][j];
		}

	}

	/* This ends the data generation phase. You will, very likely read such data from a file instead of
	 * explicitly creating it in many graphics programs.
	 */

	/*
	 *Now we give all of this data to OpenGL
	 */

	/*
	 * Essentially we create arrays that store all this, but store them on the GPU so that OpenGL can
	 *get to them quickly. These arrays are called "buffers". In this particular case, these buffers will
	 *store data about vertices, so they will be called "vertex buffers" or "vertex buffer objects (VBO)"
	 *
	 * You can use one buffer to store all the data (as we have done above) or you can use different
	 *arrays, one for each vertex attribute. We take option 1.
	 *
	 *You also need a separate VBO to store the triangle indices.
	 *
	 *When you have more than one VBO corresponding to the same model, it is inconvenient to enable/disable
	 *all of them one-by-one when rendering. In this case, we can create another object that just groups
	 *all these VBOs together and remembers which ones are enabled/disabled etc. Thus, when we want to
	 *draw the model, we merely specify this one object to OpenGL instead of all the VBOs.
	 *
	 *This "grouping object" is called a "vertex access object (VAO)". Usually you will need one VAO per model
	 *
	 */



	/* we are treating the two triangles as one model, so we need only one VAO
	 *the function below takes the number of VAO IDS to create and a pointer to an array of GLuints to store these IDs
	 *this does not allocate any GPU memory, only asks for an IDs.
	 *See http://www.opengl.org/sdk/docs/man/xhtml/glGenVertexArrays.xml
	 */
	glGenVertexArrays(1, &vao);



	/*
	 *you may have several VAOs, only one of them can be used at a time.
	 *You make a VAO current with the function below.
	 *See http://www.opengl.org/sdk/docs/man3/xhtml/glBindVertexArray.xml
	 */
	glBindVertexArray(vao);



	/*
	 *now create all the VBOs you need (in this case, 2). Since you bound the VAO above,
	 *all these VBOs will be associated with the above VAO
	 *
	 *This does not allocate any GPU memory, only asks for IDs.'
	 * See http://www.opengl.org/sdk/docs/man/xhtml/glGenBuffers.xml
	 */

	glGenBuffers(NumBuffers, vbo);




	/*
	 *Now we fill the data in these VBOs
	 */

	/*
	 *Once again only one VBO can be used at a time, so we first say which VBO
	 *we intend to use and then use it
	 */
	//starting with the VBO for all the vertex data
	// See http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, vbo[ArrayBuffer]);



	/*
	 *This next function will populate the above VBO. The parameters are as follows:
	 *1. The type of VBO this is.
	 *2. How many bytes of data will it contain. This tells OpenGL how big a buffer it needs
	 *3. A pointer to the first byte of the array that currently has all the data
	 *4. A hint to the OpenGL driver about how often this data is likely to be changd. In this case, we
	 *   won't change that data at all.
	 *
	 * This function actually causes data to be transferred from the RAM to GPU memory. This will be done
	 * only once since we are in init() and in this program that data is never changed subsequently.
	 * Thus the rendering will be more efficient since OpenGL already has all the data on GPU, and there
	 * is no vertex data being transferred from RAM to GPU during every frame.
	 *
	 *See: http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml
	 */
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexAttribs)*vertexdata.size(), &vertexdata[0], GL_STATIC_DRAW);

	/*
	 *Next do the samem for the triangle indices. Since this buffer is storing indices and not vertex
	 *data, the type of VBO is different
	 */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[IndexBuffer]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &indices[0], GL_STATIC_DRAW);



	/*
	 *The vertex data we specified above stores all the data about a vertex together (i.e. position, color)
	 *We need to now tell OpenGL what all those bytes mean (i.e. which bytes are position, which ones are
	 *color, etc.
	 *
	 *What OpenGL expects is a way to get one attribute of all vertices (i.e. all positions). Thus it
	 *organizes vertex data according to attribute.
	 *
	 *The array we just gave it is organized as below:
	 * x1,y1,z1,w1,r1,g1,b1,x2,y2,z2,r2,g2,b2, etc.
	 *
	 *OpenGL needs an organization per attribute, ie. how will it go from one position to another?
	 *
	 *The functions below tell this organization to OpenGL, in effect creating one logical array per
	 *attribute
	 *
	 *Then when we render it, we can disable individual attributes according to what our shaders expect
	 */

	//since what we will do below concerns the vertex data, bind that VBO
	//remember that whatever you bound last will continue to be the current one until you change it!
	glBindBuffer(GL_ARRAY_BUFFER, vbo[ArrayBuffer]);

	/*
	 *In View.h we defined an enum for vertex attributes enum Attrib_IDs {vPosition=0,vColor=1};
	 * Their values (0,1) are not coincidental, but they correspond exactly to the layout of all the
	 *input variables in our shader. Look at triangles.vert. The position is at location 0 and the color
	 *is at location 1.
	 *
	 *We don't have to do this (i.e. set locations in the shader and refer to them here) in every program.
	 *There is another way to "look up" the locations, but doing it this way saves that function call.
	 */

	/*
	 *Thus going by the above discussion, we tell OpenGL how to read all the positions and then all the colors
	 */

	/*
	 *The function below sets up the position attribute, and tells OpenGL that this corresponds to the
	 *variable in location 0 in the shaders. Thus, this connects the position attributes in our VBOs
	 *to the vPosition input variable in the shader
	 *
	 * The parameters are as follows: see http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribPointer.xml
	 *1. The layout position for the attribute that we are about to specify
	 *2. How many components to this attributes. In our case we have x,y,z,w so 4.
	 *3. What is the data type of the data? In our case it is float.
	 *4. Should the data be normalized before it is used? In our case NO. We do this only for normal vectors
	 *5. How many bytes are there between the starts of attributes of adjacent vertices. In our case, this is equal to the size of the VertexAttribs structure
	 *6. From which byte of the VBO should I start reading? For us, the VertexAttribs begins with position, so index 0
	 */
	glVertexAttribPointer(vPositionLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexAttribs), BUFFER_OFFSET(0));

	//enable this attribute
	glEnableVertexAttribArray(vPositionLocation);

	/*
	 *Same as above, except that the color of the first vertex starts after the position of the first vertex. Thus the last parameter skips the first position
	 */
	glVertexAttribPointer(vColorLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttribs), BUFFER_OFFSET(4 * sizeof(GLfloat)));
	//enable this attribute
	glEnableVertexAttribArray(vColorLocation);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	//unbind the VAO, so that we don't unintentionally change the settings
	//so at this point, this VAO has two VBOs and two enabled VertexAttribPointers. It is going to remember all of that!
	glBindVertexArray(0);


	glUseProgram(0);
}

/*
 *Function called from display in main. Should contain or call all the code to draw one screen from scratch
 *entirely.
 */

void View::draw()
{
	glUseProgram(program);
	/*
	 *We would like to scale up the triangles. We use glm::scale to get the scale matrix.
	 *We pass this along as the modelview matrix to the shader, which multiplies each incoming vertex's
	 *position with this matrix
	 */
	modelView = glm::scale(glm::mat4(1.0), glm::vec3(100, 200, 200));


	/*
	 *Supply the shader with all the matrices it expects.
	 *We already have all the pointers for those matrices from "init" above
	 *The value_ptr creates a copy-by-value of the glm::mat4 into an array of floats as OpenGL expects
	 */
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(proj));
	//return;
	glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, glm::value_ptr(modelView));

	//ready to draw our "model", so simply bind its VAO
	glBindVertexArray(vao);
	//  glDrawArrays(GL_TRIANGLES,0,NumVertices);
	/*
	 *glDrawElements is a "superpower" command, that actually starts all the drawing.
	 *In effect, OpenGL must use all the VBOs to draw triangles in teh following way.
	 * For each triangle to be drawn:
	 *  Get the indices for its 3 vertices from the index VBO.
	 *  Get the position of each vertex from the vertex data VBO (the correct attribute pointer).
	 *  Get the color of each vertex from the vertex data VBO (the correct attribute pointer).
	 *  Draw this triangle using all this data.
	 *
	 *Thus glDrawElements uses many arrays and indices and does all this indexing.
	 *We could've done that ourselves, but it turns out that OpenGL is much faster at doing the indexing
	 *than running our for loops. So we let it take over.
	 *
	 *
	 *The parameters for glDrawElements are as follows: See http://www.opengl.org/sdk/docs/man/xhtml/glDrawElements.xml
	 *
	 *1. What type of OpenGL primitive would you like to draw? In our case, we want triangles such that the index array directly stores 3 indices for 1 triangle
	 *2. How many total indices should I use (i.e. at what point should I stop reading the indices VBO? We have 2 triangles with 3 indices each, so a total of 6
	 *3. What is the data type of the data in the indices VBO? In our case it is unsigned integers
	 *4. From which byte of the indices VBO should I start reading? In our case from 0.
	 *
	 *Note that this command is assuming you are talking with respect to the "current" indices VBO which indexes into the "current" vertex data VBO.
	 * The glBindVertexArrays before this makes sure all the correct VBOs are "current"
	 */

	// glPolygonMode(GL_FRONT,GL_LINE); //OUTLINES
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	//glDrawArrays(GL_TRIANGLES,1,3);

	glBindVertexArray(0);

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

void View::printShaderInfoLog(GLuint shader)
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

void View::getOpenGLVersion(int *major, int *minor)
{
	const char *verstr = (const char *)glGetString(GL_VERSION);
	if ((verstr == NULL) || (sscanf_s(verstr, "%d.%d", major, minor) != 2))
	{
		*major = *minor = 0;
	}
}

void View::getGLSLVersion(int *major, int *minor)
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
