#ifndef VECTOR_ATTRIBS_H
#define VECTOR_ATTRIBS_H

class MyVertexAttribs
{
    float mPosition[4]; //x,y,z,w
    float mColor[3]; //color as r,g,b

public:
    void setXYZW(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
    {
        mPosition[0] = x;
        mPosition[1] = y;
        mPosition[2] = z;
        mPosition[3] = w;
    }

    void setColor(GLfloat r, GLfloat g, GLfloat b)
    {
        mColor[0] = r;
        mColor[1] = g;
        mColor[2] = b;
    }

    float* getPosition() { return mPosition; };
    float* getColor() { return mColor; };

    GLfloat getX() { return mPosition[0]; };
    GLfloat getY() { return mPosition[1]; };
    GLfloat getZ() { return mPosition[2]; };

};

#endif