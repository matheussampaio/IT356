#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "../glm/glm.hpp"

namespace graphics
{
	class Light
	{
	private:
		glm::vec3 ambient,diffuse,specular;
		glm::vec4 position,spot_direction;
		float spot_cutoff;
	public:
		Light(void)
		{
		}
		~Light(void)
		{
		}

		void setAmbient(float r,float g,float b)
		{
			ambient = glm::vec3(r,g,b);
		}

		void setDiffuse(float r,float g,float b)
		{
			diffuse = glm::vec3(r,g,b);
		}

		void setSpecular(float r,float g,float b)
		{
			specular = glm::vec3(r,g,b);
		}

		void setPosition(float x,float y,float z)
		{
			position = glm::vec4(x,y,z,1.0f);
		}

		void setDirection(float x,float y,float z)
		{
			position = glm::vec4(x,y,z,0.0f);
		}

		void setAmbient(glm::vec3 color)
		{
			ambient = color;
		}

		void setDiffuse(glm::vec3 color)
		{
			diffuse = color;
		}

		void setSpecular(glm::vec3 color)
		{
			specular = color;
		}

		void setPosition(glm::vec3 pos)
		{
			position = glm::vec4(pos,1.0f);
		}

		void setDirection(glm::vec3 dir)
		{
			position = glm::vec4(dir,0.0f);
		}

		void setSpotDirection(glm::vec3 dir)
		{
			spot_direction = glm::vec4(dir,0.0f);
		}

		void setSpotAngle(float angle)
		{
			spot_cutoff = angle;
		}


		glm::vec3 getAmbient()
		{
			return ambient;
		}

		glm::vec3 getDiffuse()
		{
			return diffuse;
		}

		glm::vec3 getSpecular()
		{
			return specular;
		}

		glm::vec4 getPosition()
		{
			return position;
		}

		glm::vec4 getSpotDirection()
		{
			return spot_direction;
		}

		float getSpotAngle()
		{
			return spot_cutoff;
		}


		void init()
		{
			ambient = diffuse = specular = glm::vec3(0,0,0);
			position = glm::vec4(0,0,0,1);
			spot_cutoff = 180; //by default not a spotlight
		}
	};	
}

#endif
