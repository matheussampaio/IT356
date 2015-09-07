#ifndef COLOR4D_H
#define COLOR4D_H
#include "Color3D.h"
using namespace graphics;

namespace graphics
{
    class Color4D:public Color3D
    {
    protected:
        float a;
    public:

        Color4D() : Color3D(){a=0;}

        Color4D(float R,float G,float B,float A)
            :Color3D(R,G,B)
        {
             a=A;
        }

        Color4D(const Color4D& C)
        {
            init(C.r,C.g,C.b,C.a);
        }

        Color4D(const Color3D& c,float a)
            :Color3D(c)
        {
            this->a = a;
        }

        inline float getA() const {return a;}
        inline void setA(float a){this->a=a;}

        inline void init(float r,float g,float b,float a)
        {
            Color3D::init(r,g,b);

            if (a>=0)
            {
                    if (a<=1)
                            this->a=a;
                    else
                            this->a=1;
            }
            else
            {
                    this->a = 0;
                }
        }

        inline const Color4D lerp(const Color4D& A,const Color4D& B,float t) const
        {
            float r,g,b,a;
            r = A.r + (B.r-A.r)*t;
            g = A.g + (B.g-A.g)*t;
            b = A.b + (B.b-A.b)*t;
            a = A.a + (B.a-A.a)*t;

            if (r<0)
                    r=0;
            if (r>1)
                    r=1;
            if (g<0)
                    g=0;
            if (g>1)
                    g=1;
            if (b<0)
                    b=0;
            if (b>1)
                    b=1;
            if (a<0)
                    a=0;
            if (a>1)
                    a=1;
            return Color4D(r,g,b,a);
        }


        inline void scale(float c)
        {
            float red,green,blue,alpha;

            red = r;
            green = g;
            blue = b;
            alpha = a;

            red = c*red;
            green = c*green;
            blue = c*blue;
            alpha = c*alpha;

            r=red>1.0f?1.0f:red;
            g=green>1.0f?1.0f:green;
            b=blue>1.0f?1.0f:blue;
            a=alpha>1.0f?1.0f:alpha;
        }

    };
}
#endif // COLOR4D_H
