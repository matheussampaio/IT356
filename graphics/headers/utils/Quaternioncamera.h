#ifndef QUATERNIONCAMERA_H
#define QUATERNIONCAMERA_H

#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_SWIZZLE

#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/swizzle.hpp>

class QuaternionCamera : public Camera
{
public:
	QuaternionCamera(){}

    QuaternionCamera(float ex,float ey,float ez,float lx,float ly,float lz,float ux,float uy,float uz)
	{
		init(ex,ey,ez,lx,ly,lz,ux,uy,uz);
	}

    QuaternionCamera(const glm::vec3& eye,const glm::vec3& lookAt,const glm::vec3& up)
	{
		float ex,ey,ez,lx,ly,lz,ux,uy,uz;

		glm::vec3 lookAtDir = lookAt - eye;

		ex = eye[0];
		ey = eye[1];
		ez = eye[2];

		lx = ex + 100*(lookAtDir[0]);
		ly = ey + 100*(lookAtDir[1]);
		lz = ez + 100*(lookAtDir[2]);

		glm::vec3 u(up);
		u = glm::normalize(u);
		ux = u[0];
		uy = u[1];
		uz = u[2];

		this->init(ex,ey,ez,lx,ly,lz,ux,uy,uz);
	}

    void init(float ex,float ey,float ez,float lx,float ly,float lz,float ux,float uy,float uz)
	{
		eye = glm::vec3(ex,ey,ez);

		//create the three axes u,v,w;
		glm::vec3 u,v,w;

		w = glm::vec3(ex-lx,ey-ly,ez-lz);
		w = glm::normalize(w);

		v = glm::vec3(ux,uy,uz);
		v = glm::normalize(v);

		u = glm::cross(v,w);
		u = glm::normalize(u);

		v = glm::cross(w,u);
		v = glm::normalize(v);

		glm::quat rot1,rot2;
		glm::vec3 z = glm::vec3(0,0,1);

		//rot1 aligns the Z-axis with the W-axis
		//axis is z x w, and angle is z dot w
		float dot = glm::dot(z,w);

		if (dot>=0.999)
			rot1 = glm::angleAxis(0.0f,glm::vec3(1,0,0)); //z and w are the same, no need to rotate
		else if (dot<-0.999)
			rot1 = glm::angleAxis(180.0f,glm::vec3(0,1,0)); //z and w are exact opposite, need a 180 rotation about Y axis so as to not invert UP
		else
			rot1 = glm::angleAxis((float)(180.0f*acos(w.z)/3.14159),glm::normalize(glm::cross(z,w)));

		glm::vec4 x = glm::vec4(1,0,0,0);

		glm::vec4 xtrans = glm::mat4_cast(rot1) * x;

		//rot2 rotates about w so that the axes align with u and v
		float dotp = glm::dot(glm::vec3(glm::swizzle<glm::X,glm::Y,glm::Z>(xtrans)),u);
		dotp = (dotp>1?1:(dotp<-1?-1:dotp));
		if (fabs(dotp)>=0.9999) //angle is 0 or 180, so the cross product is going to be degenerate, but we already know the axis
		{
			rot2 = glm::angleAxis((float)(180*acos(dotp)/3.14159),w);
		}
		else
		{
			rot2 = glm::angleAxis((float)(180*acos(dotp)/3.14159),glm::normalize(glm::cross(glm::vec3(glm::swizzle<glm::X,glm::Y,glm::Z>(xtrans)),u)));
		}

		quat = rot2 * rot1;

		//quat now stores the transformation to rotate the coordinate axes into u,v,w. The camera quaternion is the exact opposite (i.e. conjugate)
		quat = glm::conjugate(quat);

		worldToCamera = glm::mat4_cast(quat) * glm::translate(glm::mat4(1.0),glm::vec3(-eye.x,-eye.y,-eye.z));
		cameraToWorld = glm::inverse(worldToCamera);

	}
    //this function takes this camera and rotates it by a given angle about a given axis and point and returns the resulting camera
    QuaternionCamera rotateBy(const glm::vec3& aboutPoint,const glm::vec3& axis,float angleDegrees)
	{
		/*
		the algorithm is as follows: we must get the new camera position and new set of axes for the camera
		Getting the new set of axes:
			1. Using "quat", get the u,v,w axes of the current camera (take its conjugate and multiply the resulting matrix to the coordinate axes, as in the comment in the above function)
			2. Get rotation matrix for rotating "angleDegrees" about "axis".
			3. Multiply u,v,w with this matrix to get new u,v,w.
		Getting the new eye:
			1. Get transformation matrix to (a) translate by -aboutPoint (b) align axis with Y axis (c) rotate by "angleDegrees" (d) unrotate (b) and (e) translate aboutPoint
			2. Multiply current eye position with matrix to get new eye position

		To convert to a quaternion, simply call constructor passing it eye, -new z (lookAt) and new v (up)
		*/

		glm::vec4 u,v,w;

		u = glm::vec4(1.0f,0.0f,0.0f,0.0f);
		v = glm::vec4(0.0f,1.0f,0.0f,0.0f);
		w = glm::vec4(0.0f,0.0f,1.0f,0.0f);

		//transform u v w by cameraToWorld to get them in world coordinates
		u = cameraToWorld * u;
		v = cameraToWorld * v;
		w = cameraToWorld * w;

		//get matrix to rotate "angleDegrees" about "axis"
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0),angleDegrees,axis);

		//rotate u v w by rotate to get new u v w
		u = rotate * u;
		v = rotate * v;
		w = rotate * w;

		//now get the new eye

		glm::mat4 alignAxisToY;

		//to align axis to Y it must be rotated by axis.Y about the vector axis * Y
		glm::vec3 Y = glm::vec3(0,1,0);
		glm::vec3 tempAxis = glm::cross(axis,Y);
		float angle = 180.0f*acos(glm::dot(axis,Y)/glm::length(axis))/3.14159f;

		if (angle==0) //axis is Y
			alignAxisToY = glm::mat4(1.0);
		else if (angle==180) //axis is -Y
			alignAxisToY = glm::rotate(glm::mat4(1.0),180.0f,glm::vec3(1,0,0));
		else
			alignAxisToY = glm::rotate(glm::mat4(1.0),angle,tempAxis);

		//transformation for the eye as per point 2 above
		glm::mat4 eyeTrans = glm::translate(glm::mat4(1.0),glm::vec3(aboutPoint[0],aboutPoint[1],aboutPoint[2])) * glm::inverse(alignAxisToY) * glm::rotate(glm::mat4(1.0),angleDegrees,glm::vec3(0,1,0)) * alignAxisToY * glm::translate(glm::mat4(1.0),glm::vec3(-aboutPoint[0],-aboutPoint[1],-aboutPoint[2]));

		glm::vec4 neweye = glm::vec4(eye,1.0);
		neweye = eyeTrans * neweye;

		return QuaternionCamera(glm::swizzle<glm::X,glm::Y,glm::Z>(neweye),glm::vec3(glm::swizzle<glm::X,glm::Y,glm::Z>(neweye))-glm::vec3(glm::swizzle<glm::X,glm::Y,glm::Z>(w))*50.0f,glm::swizzle<glm::X,glm::Y,glm::Z>(v));

	}

    //this function zooms in the direction of lookAt by a specific camera. Quaternion does not change
    QuaternionCamera zoomBy(float distance)
	{
		//first get the camera axes in the world coordinate system

		glm::vec4 u,v,w;

		u = glm::vec4(1,0,0,0);
		v = glm::vec4(0,1,0,0);
		w = glm::vec4(0,0,1,0);

		//transform u v w by cameraToWorld to get them in world coordinates
		u = cameraToWorld * u;
		v = cameraToWorld * v;
		w = cameraToWorld * w;

		//get new eye by translating it by "distance" along -w
		glm::vec3 neweye;

		neweye = eye - glm::vec3(glm::swizzle<glm::X,glm::Y,glm::Z>(w)) * distance;

		return QuaternionCamera(neweye,-glm::vec3(glm::swizzle<glm::X,glm::Y,glm::Z>(w)),glm::swizzle<glm::X,glm::Y,glm::Z>(v));
	}

    inline static QuaternionCamera interp(const QuaternionCamera& a,const QuaternionCamera& b,float t)
    {
        glm::quat q = glm::slerp(a.quat,b.quat,t);
        glm::vec3 e = a.eye + (b.eye-a.eye)*t;
        return QuaternionCamera(q,e);

    }

	inline glm::vec3 getLookDirection()
	{
		glm::vec4 a = glm::vec4(0.0f,0.0f,-1.0f,0.0f);

		a = cameraToWorld * a;
	
		return glm::swizzle<glm::X,glm::Y,glm::Z>(a);
	}

	inline glm::vec3 getRightDirection()
	{
		glm::vec4 a = glm::vec4(1.0f,0.0f,0.0f,0.0f);

		a = cameraToWorld * a;
	
		return glm::swizzle<glm::X,glm::Y,glm::Z>(a);
	}

	inline glm::vec3 getUpDirection()
	{
		glm::vec4 a = glm::vec4(0.0f,1.0f,0.0f,0.0f);

		a = cameraToWorld * a;
	
		return glm::swizzle<glm::X,glm::Y,glm::Z>(a);
	}


    const bool operator == (const QuaternionCamera& cam) const
    {
        return ((fabs(glm::distance(cam.eye,eye))<0.001) && (cam.quat == quat));
    }



private:
    QuaternionCamera(glm::quat& q,glm::vec3& e)
	{
		quat = q;
		eye = e;
		worldToCamera = glm::mat4_cast(quat) * glm::translate(glm::mat4(1.0),glm::vec3(-eye.x,-eye.y,-eye.z));
		cameraToWorld = glm::inverse(worldToCamera);
	}

    glm::quat quat;
};

#endif // QUATERNIONCAMERA_H
