#ifndef _TRANSFORMNODE_H_
#define _TRANSFORMNODE_H_

#include "Node.h"
#include <glm/glm.hpp>

class TransformNode : public Node
{
public:

	TransformNode(Scenegraph *graph,string name="")
		:Node(graph,name)
	{
		transform = glm::mat4(1.0);
		animation_transform = glm::mat4(1.0);
		child = NULL;
	}

	~TransformNode(void)
	{
		if (child!=NULL)
			delete child;
	}

	virtual Node *clone()
	{
		Node *newchild;

		if (child!=NULL)
		{
			newchild = child->clone();
		}
		else
		{
			newchild = NULL;
		}

		TransformNode *newtransform = new TransformNode(scenegraph,name);
		newtransform->setTransform(transform);
		newtransform->setAnimationTransform(animation_transform);

		if (newchild!=NULL)
		{
			newtransform->setChild(newchild);
		}		
		return newtransform;
	}

	virtual Node *getNode(string name)
	{
		if (Node::getNode(name)!=NULL)
			return Node::getNode(name);

		if (child!=NULL)
		{
			return child->getNode(name);
		}

		return NULL;
	}

	void setChild(Node *child)
	{
		this->child = child;
		this->child->setParent(this);
	}

	virtual void draw(stack<glm::mat4> &modelView)
    {
        modelView.push(modelView.top());
        modelView.top() = modelView.top() * animation_transform * transform;
        if (child!=NULL)
			child->draw(modelView);
        modelView.pop();
    }

	void setTransform(glm::mat4 obj)
    {
        transform = obj;
    }

    void setAnimationTransform(glm::mat4 mat)
    {
        animation_transform = mat;
    }

    glm::mat4 getTransform()
    {
        return transform;
    }

    glm::mat4 getAnimationTransform()
    {
        return animation_transform;
    }

	void setScenegraph(Scenegraph *graph)
	{
		Node::setScenegraph(graph);
		if (child!=NULL)
		{
			child->setScenegraph(graph);
		}
	}

protected:
	glm::mat4 transform,animation_transform;
	Node *child;
};

#endif