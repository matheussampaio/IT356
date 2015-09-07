#ifndef COLOR3D_H
#define COLOR3D_H

#include <cmath>

namespace graphics
{
    class Color3D
    {
    public:
        float r,g,b;
    public:

        Color3D() {r=g=b=0;}

        Color3D(float red,float green,float blue)
        {
            this->r=red;
            this->g=green;
            this->b=blue;

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

        }

        inline float getR() const {return r;}
        inline float getG() const {return g;}
        inline float getB() const {return b;}
        inline void setR(float r){this->r=r;}
        inline void setG(float g){this->g=g;}
        inline void setB(float b){this->b=b;}

        inline void init(float r,float g,float b)
        {
            if (r>=0)
            {
                if (r<=1)
                   this->r=r;
                else
                   this->r=1;
            }
            else
            {
                this->r = 0;
            }
            if (g>=0)
            {
                if (g<=1)
                    this->g=g;
                else
                    this->g=1;
            }
            else
            {
                this->g = 0;
            }

            if (b>=0)
            {
                if (b<=1)
                    this->b=b;
                else
                    this->b=1;
            }
            else
            {
                this->b = 0;
            }
        }

        inline const Color3D lerp(Color3D& A,Color3D& B,float t)
        {
            float r,g,b;
            r = A.r + (B.r-A.r)*t;
            g = A.g + (B.g-A.g)*t;
            b = A.b + (B.b-A.b)*t;

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

            return Color3D(r,g,b);
        }

        inline float getIntensity()
        {
                return sqrtf((r*r + g*g + b*b)/3);
        }

        inline const Color3D operator +(const Color3D& col) const
        {
                float red,green,blue;

                if (r + col.r<=1.0f)
                        red = r + col.r;
                else
                        red = 1.0f;

                if (g + col.g<=1.0f)
                        green = g + col.g;
                else
                        green = 1.0f;

                if (b + col.b<=1.0f)
                        blue = b + col.b;
                else
                        blue = 1.0f;

                return Color3D(red,green,blue);
        }

        inline const Color3D operator -(const Color3D& col) const
        {
                //difference operator
                float red,green,blue;

                if (fabs(r - col.r)>=0)
                        red = (float)fabs(r - col.r);
                else
                        red = 0;

                if (fabs(g - col.g)>=0)
                        green = (float)fabs(g - col.g);
                else
                        green = 0;

                if (fabs(b - col.b)>=0)
                        blue = (float)fabs(b - col.b);
                else
                        blue = 0;

                return Color3D(red,green,blue);
        }

        inline void scale(float c)
        {
                float red,green,blue;

                red = r;
                green = g;
                blue = b;

                red = c*red;
                green = c*green;
                blue = c*blue;

                r=red>1.0f?1.0f:red;
                g=green>1.0f?1.0f:green;
                b=blue>1.0f?1.0f:blue;
        }

    };
}

#endif // COLOR3D_H
