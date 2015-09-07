#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"


class Camera
{
public:
    Camera()
    {
        worldToCamera = glm::mat4(1.0);
        cameraToWorld = glm::mat4(1.0);
    }

    Camera(float ex,float ey,float ez,float lx,float ly,float lz,float ux,float uy,float uz)
    {
        init(ex,ey,ez,lx,ly,lz,ux,uy,uz);
    }

    Camera(const glm::vec3& eye,const glm::vec3& lookAt,const glm::vec3& up)
    {
        worldToCamera = glm::lookAt(eye,lookAt,up);
        cameraToWorld = glm::inverse(worldToCamera);
    }

    ~Camera()
    {
    }

    const glm::vec3 getEye() const
    {
        return eye;
    }

    inline virtual void init(float ex,float ey,float ez,float lx,float ly,float lz,float ux,float uy,float uz)
    {
        worldToCamera = glm::lookAt(glm::vec3(ex,ey,ez),glm::vec3(lx,ly,lz),glm::vec3(ux,uy,uz));
        cameraToWorld = glm::inverse(worldToCamera);
    }

    glm::mat4 getCameraTransform()
    {
        return worldToCamera;
    }    

    const glm::mat4& getConstCameraTransform() const
    {
        return worldToCamera;
    }

    glm::mat4& getInvCameraTransform()
    {
        return cameraToWorld;
    }

    const glm::mat4& getConstInvCameraTransform() const
    {
        return cameraToWorld;
    }



protected:
    glm::vec3 eye;
    glm::mat4 worldToCamera;
    glm::mat4 cameraToWorld;
};

#endif // CAMERA_H
