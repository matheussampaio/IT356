#ifndef VECTOR_ATTRIBS_H
#define VECTOR_ATTRIBS_H

class VertexAttribs
{
public:

    VertexAttribs(GLfloat x, GLfloat y)
    {
        position[0] = x;
        position[1] = y;
        position[2] = 0;
        position[3] = 1;

        color[0] = 0.0f;
        color[1] = 0.0f;
        color[2] = 0.0f;
    }

    float position[4]; //x,y,z,w
    float color[3]; //color as r,g,b
};

#endif