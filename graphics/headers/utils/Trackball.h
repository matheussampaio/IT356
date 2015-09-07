#ifndef _TRACKBALL_H_
#define _TRACKBALL_H_

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include <cmath>
namespace graphics
{
	class Trackball
	{
	public:

		Trackball(void)
		{
			radius = 20000;
			center = glm::vec3(0.0f,0.0f,0.0f);
			transform = glm::mat4(1.0f);
		}

		Trackball(glm::vec3 center,float r)
		{
			radius = r;
			this->center = center;
			transform = glm::mat4(1.0f);
		}

		void initRadius(float r)
		{
			radius = r;
		}

		void initCenter(glm::vec3 center)
		{
			this->center = center;
		}

		~Trackball(void)
		{
		}

		inline void change(int dx,int dy)
		{
			float dist = sqrt((float)dx*dx + (float)dy*dy);
			float angle = 180.0f*(dist/radius)/3.14159f;
			glm::vec3 axis((float)-dy,(float)dx,0.0f);

			transform = glm::translate(glm::mat4(1.0f),center) * glm::rotate(glm::mat4(1.0),angle,axis) * glm::translate(glm::mat4(1.0f),-center) * transform;


		}

		inline glm::mat4 getTransform()
		{
			return transform;
		}

	private:
		float radius;
		glm::vec3 center;
		glm::mat4 transform;
	};
}

#endif