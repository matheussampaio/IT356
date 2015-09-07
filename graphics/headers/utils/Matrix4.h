#ifndef _MATRIX_H
#define _MATRIX_H

#include "../../geometry/headers/Vector3D.h"
#include "../../geometry/headers/Segment3D.h"
using namespace geometry;

namespace graphics
{
    class Matrix4
    {
    private:
            float m[4][4];
            float openglm[16]; //the same matrix as above, but in column major form for opengl
            bool updated;


    public:
            Matrix4(void)
            {           
                setIdentity();
                updated = false;
            }

            Matrix4(float a00,float a01,float a02,float a03,float a10,float a11,float a12,float a13,float a20,float a21,float a22,float a23,float a30,float a31,float a32,float a33)
            {
                updated = false;
                init(a00,a01,a02,a03,a10,a11,a12,a13,a20,a21,a22,a23,a30,a31,a32,a33);
            }

            ~Matrix4(void)
            {

            }

            Matrix4(const Matrix4& a)
            {
                int i,j;

                updated = false;
                for (i=0;i<4;i++)
                {
                    for (j=0;j<4;j++)
                    {
                        m[i][j] = a.m[i][j];
                    }
                }              
            }

            const Matrix4 operator=(const Matrix4 a)
            {
                int i,j;
                for (i=0;i<4;i++)
                {
                    for (j=0;j<4;j++)
                    {
                            m[i][j] = a.m[i][j];
                    }
                }
                updated = false;
                return *this;
            }


            inline void setIdentity()
            {
                int i,j;

                for (i=0;i<4;i++)
                {
                    for (j=0;j<4;j++)
                    {
                        if (i==j)
                            m[i][j] = 1.0f;
                        else
                            m[i][j] = 0.0f;
                    }
                }
                updated = false;
            }

            inline void initFromOpenGL(float om[16])
            {
                int i,j;

                for (i=0;i<4;i++)
                {
                    for (j=0;j<4;j++)
                    {
                        m[j][i] = om[4*i+j];  //opengl stores matrices in column major order
                    }
                }
                update();
            }

            inline void init(float a00,float a01,float a02,float a03,float a10,float a11,float a12,float a13,float a20,float a21,float a22,float a23,float a30,float a31,float a32,float a33)
            {
                m[0][0] = a00;
                m[0][1] = a01;
                m[0][2] = a02;
                m[0][3] = a03;

                m[1][0] = a10;
                m[1][1] = a11;
                m[1][2] = a12;
                m[1][3] = a13;

                m[2][0] = a20;
                m[2][1] = a21;
                m[2][2] = a22;
                m[2][3] = a23;

                m[3][0] = a30;
                m[3][1] = a31;
                m[3][2] = a32;
                m[3][3] = a33;
                updated = false;
            }

            inline void init(Matrix4& a)
            {
                int i,j;

                for (i=0;i<4;i++)
                {
                    for (j=0;j<4;j++)
                    {
                        m[i][j] = a.m[i][j];
                    }
                }
                updated = false;
            }

            float *getOpenGLMatrix()
            {
                update();
                return openglm;
            }

            const Matrix4 operator +(const Matrix4& a) const
            {
                Matrix4 c;
                int i,j;

                for (i=0;i<4;i++)
                {
                    for (j=0;j<4;j++)
                    {
                        c.m[i][j] = m[i][j] + a.m[i][j];
                    }
                }
                return c;
            }

            const Matrix4 operator -(const Matrix4& a) const
            {
                Matrix4 c;
                int i,j;

                for (i=0;i<4;i++)
                {
                    for (j=0;j<4;j++)
                    {
                         c.m[i][j] = m[i][j] - a.m[i][j];
                    }
                }
                return c;
            }
            const Matrix4 operator *(const Matrix4& a) const
            {
                Matrix4 c;
                int i,j,k;

                for (i=0;i<4;i++)
                {
                    for (j=0;j<4;j++)
                    {
                        c.m[i][j] = 0.0f;
                        for (k=0;k<4;k++)
                        {
                            c.m[i][j] = c.m[i][j] + m[i][k]*a.m[k][j];
                        }
                    }
                }
                return c;
            }

            Matrix4 inverse() const
            {
                float minv[16];
                Matrix4 c;

                if (helper_inverse(minv))
                    c.initFromOpenGL(minv);

                return c;
            }

            const Matrix4 transpose()
            {
                Matrix4 c;
                int i,j;

                for (i=0;i<4;i++)
                {
                    for (j=0;j<4;j++)
                    {
                        c.m[i][j] = m[j][i];
                    }
                }
                return c;
            }

            static const Matrix4 getTranslate(float tx,float ty,float tz)
            {
                Matrix4 c;
                c.setIdentity();
                c.m[0][3] = tx;
                c.m[1][3] = ty;
                c.m[2][3] = tz;

                return c;
            }

            static const Matrix4 getScale(float sx,float sy,float sz)
            {
                Matrix4 c;
                c.setIdentity();
                c.m[0][0] = sx;
                c.m[1][1] = sy;
                c.m[2][2] = sz;

                return c;
            }

            static const Matrix4 getRotate(float degrees,float ax,float ay,float az)
            {
                Vector3D u,v,w;
                Matrix4 a,b,c;
                float angle = 3.14159f*degrees/180;

                w.init(ax,ay,az);
                w.normalize();

                v.init(w.getY()-w.getZ(),w.getZ()-w.getX(),w.getX()-w.getY());
                v.normalize();

                u = v*w;

                c.init(u.getX(),u.getY(),u.getZ(),0,v.getX(),v.getY(),v.getZ(),0,w.getX(),w.getY(),w.getZ(),0,0,0,0,1);
                a.init(u.getX(),v.getX(),w.getX(),0,u.getY(),v.getY(),w.getY(),0,u.getZ(),v.getZ(),w.getZ(),0,0,0,0,1);
                b.init(cos(angle),-sin(angle),0,0,sin(angle),cos(angle),0,0,0,0,1,0,0,0,0,1);

                 return a*b*c;
            }

            inline void multPos(double point[3]) const
            {
                    double v[4],w[4];
                    int i,j;

                    for (i=0;i<3;i++)
                            v[i] = point[i];
                    v[3] = 1.0f;

                    for (i=0;i<4;i++)
                    {
                        w[i] = 0.0f;
                        for (j=0;j<4;j++)
                        {
                            w[i] = w[i] + m[i][j]*v[j];
                        }
                    }

                    for (i=0;i<3;i++)
                            point[i] = w[i]/w[3];
            }

            inline void multPos(float point[3]) const
            {
                    float v[4],w[4];
                    int i,j;

                    for (i=0;i<3;i++)
                            v[i] = point[i];
                    v[3] = 1.0f;

                    for (i=0;i<4;i++)
                    {
                        w[i] = 0.0f;
                        for (j=0;j<4;j++)
                        {
                            w[i] = w[i] + m[i][j]*v[j];
                        }
                    }

                    for (i=0;i<3;i++)
                            point[i] = w[i]/w[3];
            }

            inline void multVec(float vec[3]) const
            {
                    float v[4],w[4];
                    int i,j;

                    for (i=0;i<3;i++)
                            v[i] = vec[i];
                    v[3] = 0.0f;

                    for (i=0;i<4;i++)
                    {
                            w[i] = 0.0f;
                            for (j=0;j<4;j++)
                            {
                                    w[i] = w[i] + m[i][j]*v[j];
                            }
                    }

                    for (i=0;i<3;i++)
                            vec[i] = w[i];
            }

            inline void multVec(double vec[3]) const
            {
                    double v[4],w[4];
                    int i,j;

                    for (i=0;i<3;i++)
                            v[i] = vec[i];
                    v[3] = 0.0f;

                    for (i=0;i<4;i++)
                    {
                            w[i] = 0.0f;
                            for (j=0;j<4;j++)
                            {
                                    w[i] = w[i] + m[i][j]*v[j];
                            }
                    }

                    for (i=0;i<3;i++)
                            vec[i] = w[i];
            }

    private:
            bool helper_inverse(float minv[16]) const
            {

                    float r1[8], r2[8], r3[8], r4[8];
                    float *s[4], *tmprow;

                    s[0] = &r1[0];
                    s[1] = &r2[0];
                    s[2] = &r3[0];
                    s[3] = &r4[0];

                    register int i,j,p,jj;
                    for(i=0;i<4;i++)
                    {
                            for(j=0;j<4;j++)
                            {
                                    s[i][j] = m[i][j];
                                    if(i==j) s[i][j+4] = 1.0;
                                    else     s[i][j+4] = 0.0;
                            }
                    }
                    float scp[4];
                    for(i=0;i<4;i++)
                    {
                            scp[i] = fabs(s[i][0]);
                            for(j=1;j<4;j++)
                                    if(fabs(s[i][j]) > scp[i])
                                            scp[i] = fabs(s[i][j]);
                                    if(scp[i] == 0.0)
                                            return false; // singular matrix!
                    }

                    int pivot_to;
                    float scp_max;
                    for(i=0;i<4;i++)
                    {
                            // select pivot row
                            pivot_to = i;
                            scp_max = fabs(s[i][i]/scp[i]);
                            // find out which row should be on top
                            for(p=i+1;p<4;p++)
                                    if(fabs(s[p][i]/scp[p]) > scp_max)
                                    {
                                            scp_max = fabs(s[p][i]/scp[p]);
                                            pivot_to = p;
                                    }
                                    // Pivot if necessary
                                    if(pivot_to != i)
                                    {
                                            tmprow = s[i];
                                            s[i] = s[pivot_to];
                                            s[pivot_to] = tmprow;
                                            float tmpscp;
                                            tmpscp = scp[i];
                                            scp[i] = scp[pivot_to];
                                            scp[pivot_to] = tmpscp;
                                    }

                                    float mji;
                                    // perform gaussian elimination
                                    for(j=i+1;j<4;j++)
                                    {
                                            mji = s[j][i]/s[i][i];
                                            s[j][i] = 0.0;
                                            for(jj=i+1;jj<8;jj++)
                                                    s[j][jj] -= mji*s[i][jj];
                                    }
                    }
                    if(s[3][3] == 0.0)
                            return false; // singular matrix!

                    //
                    // Now we have an upper triangular matrix.
                    //
                    //  x x x x | y y y y
                    //  0 x x x | y y y y
                    //  0 0 x x | y y y y
                    //  0 0 0 x | y y y y
                    //
                    //  we'll back substitute to get the inverse
                    //
                    //  1 0 0 0 | z z z z
                    //  0 1 0 0 | z z z z
                    //  0 0 1 0 | z z z z
                    //  0 0 0 1 | z z z z
                    //

                    float mij;
                    for(i=3;i>0;i--)
                    {
                            for(j=i-1;j > -1; j--)
                            {
                                    mij = s[j][i]/s[i][i];
                                    for(jj=j+1;jj<8;jj++)
                                            s[j][jj] -= mij*s[i][jj];
                            }
                    }

                    for(i=0;i<4;i++)
                            for(j=0;j<4;j++)
                                    minv[j*4+i] = s[i][j+4] / s[i][i];

                    return true;
            }

            void update()
            {
                if (!updated)
                {
                    int i,j;

                    for (i=0;i<4;i++)
                    {
                        for (j=0;j<4;j++)
                        {
                            openglm[4*j+i] = m[i][j];
                        }
                    }
                    updated = true;
                }
            }


    };
}
#endif
