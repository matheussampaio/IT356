#ifndef NODE_H
#define NODE_H

#include <string>
#include <map>
#include <stack>
using namespace std;
#include <glm/glm.hpp>

class Scenegraph;

class Node
{
protected:
    string name;
    Scenegraph *scenegraph;
	bool bbDraw;
public:
    Node(Scenegraph *graph,string name="")
    {
		this->parent = NULL;
        scenegraph = graph;
        setName(name);
    }

    virtual ~Node()
    {
	}

	virtual Node *getNode(string name)
	{
		if (this->name == name)
			return this;

		return NULL;
	}

    virtual void draw(stack<glm::mat4>& modelView)=0;
	virtual Node *clone()=0;
	void setParent(Node *parent)
	{
		this->parent = parent;
	}

	virtual void setScenegraph(Scenegraph *graph)
	{
		this->scenegraph = graph;
	}

    void setName(const string& name)
    {
        this->name = name;
    }

	void setBBDraw(bool d)
	{
		bbDraw = d;
	}

protected:
	Node *parent;
};

#endif // NODE_H
