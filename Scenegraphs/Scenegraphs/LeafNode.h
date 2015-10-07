#ifndef _LEAF_H_
#define _LEAF_H_
#include "node.h"
#include <utils/Object.h>
#include <utils/Material.h>

class LeafNode : public Node
{
public:

	LeafNode(graphics::Object *instanceOf,Scenegraph *graph,string name="")
		:Node(graph,name)
	{
		this->instanceOf = instanceOf;
		//default material
		material.setAmbient(1.0f,0.6f,0.6f);
		material.setDiffuse(1.0f,0.6f,0.6f);
		material.setSpecular(0.2f,0.1f,0.1f);
		material.setShininess(1);
	}

	~LeafNode(void)
	{
	}

	Node *clone()
	{
		LeafNode *newclone = new LeafNode(instanceOf,scenegraph,name);
		newclone->setMaterial(material);

		return newclone;
	}

	virtual void draw(stack<glm::mat4> &modelView)
    {
		GLuint a;
        if (instanceOf!=NULL)
		{
			 //get the color
            glm::vec4 color = material.getAmbient();

            //set the color for all vertices to be drawn for this object
            glUniform3fv(scenegraph->objectColorLocation,1,glm::value_ptr(color));
			a = glGetError();
			glUniformMatrix4fv(scenegraph->modelviewLocation,1,GL_FALSE,glm::value_ptr(modelView.top()));
			a = glGetError();
			instanceOf->draw(GL_TRIANGLES);        
			a = glGetError();
		}
    }
	

	glm::vec4 getColor()
	{
		return material.getAmbient();
	}

	/*
		*Set the material of each vertex in this object
		*/
	virtual void setMaterial(graphics::Material& mat)
	{
		material = mat;
	}

	/*
		* gets the material
		*/
	graphics::Material getMaterial()
	{
		return material;
	}

protected:
	graphics::Object *instanceOf;
	graphics::Material material;
};
#endif
