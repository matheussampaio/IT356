#include "Scenegraph.h"
#include <stack>
#include "TransformNode.h"
using namespace std;
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

Scenegraph::Scenegraph()
{
    root = NULL;
}

void Scenegraph::makeScenegraph(Node *r)
{
    if (root!=NULL)
    {
        delete root;
        root = NULL;
    }
    this->root = r;
	if (this->root!=NULL)
		this->root->setScenegraph(this);

}



Scenegraph::~Scenegraph()
{
    if (root!=NULL)
    {
        delete root;
        root = NULL;
    }
}

void Scenegraph::initShaderProgram(GLint shaderProgram)
{
   modelviewLocation = glGetUniformLocation(shaderProgram,"modelview");
   objectColorLocation = glGetUniformLocation(shaderProgram,"vColor");
}

void Scenegraph::draw(stack<glm::mat4>& modelView)
{
    if (root!=NULL)
    {
        root->draw(modelView);
    }
}

void Scenegraph::animate(float time)
{
	
	
}

