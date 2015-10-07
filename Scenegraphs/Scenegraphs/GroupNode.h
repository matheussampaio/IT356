#ifndef GROUPNODE_H
#define GROUPNODE_H

#include <GL/gl.h>
#include <vector>
using namespace std;
#include "Node.h"

class GroupNode: public Node
{
protected:
    vector<Node *> children;

public:
    GroupNode(Scenegraph *graph,string name="")
        :Node(graph,name)
    {

    }

    ~GroupNode()
    {
        for (unsigned int i=0;i<children.size();i++)
            delete children[i];
    }

	virtual Node *clone()
	{
		vector<Node *> newc;

		for (int i=0;i<children.size();i++)
		{
			newc.push_back(children[i]->clone());
		}

		GroupNode * newgroup = new GroupNode(scenegraph,name);

		for (int i=0;i<children.size();i++)
		{
			newgroup->addChild(newc[i]);
		}		
		return newgroup;

	}

	virtual Node* getNode(string name)
	{
		if (Node::getNode(name)!=NULL)
		{
			return Node::getNode(name);
		}

		int i=0;
		Node *answer = NULL;

		while ((i<children.size()) && (answer == NULL))
		{
			answer = children[i]->getNode(name);
			i++;
		}
		return answer;
	}

	void setScenegraph(Scenegraph * graph)
	{
		Node::setScenegraph(graph);
		for (int i=0;i<children.size();i++)
		{
			children[i]->setScenegraph(graph);
		}
	}

    virtual void draw(stack<glm::mat4> &modelView)
    {
        for (int i=0;i<children.size();i++)
        {
            children[i]->draw(modelView);
        }
    }

    void addChild(Node *child)
    {
        children.push_back(child);
		child->setParent(this);
    }

    const vector<Node *> getChildren() const
    {
        return children;
    }
};

#endif // GROUP_H
