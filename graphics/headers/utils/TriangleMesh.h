#ifndef _TRIANGLE_MESH_H_
#define _TRIANGLE_MESH_H_

#include "../glm/glm.hpp"
#include <vector>
using namespace std;
#include "Material.h"


class VertexAttribs
{
public:
	glm::vec4 position;
	glm::vec4 normal;
	glm::vec3 texcoords;
	//the tangent at the point, in object coordinates
    glm::vec4 tangent;
};


class TriangleMesh
{
	friend class OBJImporter;
	friend class OBJExporter;

public:
	TriangleMesh(void)
	{

	}
	~TriangleMesh(void)
	{}

	inline glm::vec4 getMinimumBounds()
	{
		return minBounds;
	}

	inline glm::vec4 getMaximumBounds()
	{
		return maxBounds;
	}

	inline vector<VertexAttribs>& getVertexData()
	{
		return vertex_data;
	}



	inline vector<unsigned int>& getTriangles()
	{
		return triangles;
	}

	inline vector<unsigned int>& getTriangleTextureIndices()
	{
		return triangle_texture_indices;
	}

	inline vector<unsigned int>& getTriangleNormalIndices()
	{
		return triangle_normal_indices;
	}

	inline void initVertexData(vector<VertexAttribs>& p)
	{
		vertex_data = p;
		computeBoundingBox();
	}


	inline void initTriangleTextureIndices(vector<unsigned int>& triangles)
	{
		this->triangle_texture_indices = triangles;
	}

	inline void initTriangleNormalIndices(vector<unsigned int>& triangles)
	{
		this->triangle_normal_indices = triangles;
	}

	inline void initTriangles(vector<unsigned int>& triangles)
	{
		this->triangles = triangles;
	}


	inline void computeBoundingBox()
	{
		unsigned int i,j;

		if (vertex_data.size()<=0)
			return;

		minBounds = maxBounds = vertex_data[0].position;
		for (i=0;i<vertex_data.size();i++)
		{
			for (j=0;j<3;j++)
			{
				if (vertex_data[i].position[j]<minBounds[j])
				{
					minBounds[j] = vertex_data[i].position[j];
				}

				if (vertex_data[i].position[j]>maxBounds[j])
				{
					maxBounds[j] = vertex_data[i].position[j];
				}
			}
		}
	}

	void computeNormals()
	{
		unsigned int i,k;

		for (i=0;i<vertex_data.size();i++)
		{
			vertex_data[i].normal = glm::vec4(0,0,0,0.0);
		}

		for (i=0;i<triangles.size();i+=3)
		{
			glm::vec4 norm;

			//compute the normal of this triangle
			int v[3];

			for (k=0;k<3;k++)
			{
				v[k] = triangles[i+k];
			}

			//the newell's method to calculate normal
			
			for (k=0;k<3;k++)
			{
				norm[0] += (vertex_data[v[k]].position[1]-vertex_data[v[(k+1)%3]].position[1])*(vertex_data[v[k]].position[2]+vertex_data[v[(k+1)%3]].position[2]);
				norm[1] += (vertex_data[v[k]].position[2]-vertex_data[v[(k+1)%3]].position[2])*(vertex_data[v[k]].position[0]+vertex_data[v[(k+1)%3]].position[0]);
				norm[2] += (vertex_data[v[k]].position[0]-vertex_data[v[(k+1)%3]].position[0])*(vertex_data[v[k]].position[1]+vertex_data[v[(k+1)%3]].position[1]);
			}

			if (glm::length(norm)>0)
				norm = glm::normalize(norm);

			for (k=0;k<3;k++)
			{
				vertex_data[v[k]].normal = vertex_data[v[k]].normal + norm;
			}
		}

		for (i=0;i<vertex_data.size();i++)
		{
			if (glm::length(vertex_data[i].normal)>0)
				vertex_data[i].normal = glm::normalize(vertex_data[i].normal);
		}
	}

	void computeTangents()
    {
        int i,j;

        for (i=0;i<vertex_data.size();i++)
        {
            for (j=0;j<4;j++)
                vertex_data[i].tangent[j] = 0.0f;
        }

        //go through all the triangles
        for (i=0;i<triangles.size();i+=3)
        {
            int i0,i1,i2;

            i0 = triangles[i];
            i1 = triangles[i+1];
            i2 = triangles[i+2];

            glm::vec3 v0 = glm::vec3(vertex_data[i0].position[0],vertex_data[i0].position[1],vertex_data[i0].position[2]);
            glm::vec3 v1 = glm::vec3(vertex_data[i1].position[0],vertex_data[i1].position[1],vertex_data[i1].position[2]);
            glm::vec3 v2 = glm::vec3(vertex_data[i2].position[0],vertex_data[i2].position[1],vertex_data[i2].position[2]);

            // Shortcuts for UVs
            glm::vec2 uv0 = glm::vec2(vertex_data[i0].texcoords[0],vertex_data[i0].texcoords[1]);
            glm::vec2 uv1 = glm::vec2(vertex_data[i1].texcoords[0],vertex_data[i1].texcoords[1]);
            glm::vec2 uv2 = glm::vec2(vertex_data[i2].texcoords[0],vertex_data[i2].texcoords[1]);

            // Edges of the triangle : postion delta
            glm::vec3 deltaPos1 = v1-v0;
            glm::vec3 deltaPos2 = v2-v0;

            // UV delta
            glm::vec2 deltaUV1 = uv1-uv0;
            glm::vec2 deltaUV2 = uv2-uv0;

            float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
            glm::vec3 tangent = (deltaPos1 * deltaUV2.y   - deltaPos2 * deltaUV1.y)*r;

            for (j=0;j<3;j++)
            {
                vertex_data[triangles[i+j]].tangent[0] += tangent.x;
                vertex_data[triangles[i+j]].tangent[1] += tangent.y;
                vertex_data[triangles[i+j]].tangent[2] += tangent.z;
            }
        }

        for (i=0;i<vertex_data.size();i++)
        {
            glm::vec3 t = glm::vec3(vertex_data[i].tangent[0],vertex_data[i].tangent[1],vertex_data[i].tangent[2]);
            t = glm::normalize(t);
            glm::vec3 n = glm::vec3(vertex_data[i].normal[0],vertex_data[i].normal[1],vertex_data[i].normal[2]);

            glm::vec3 b = glm::cross(n,t);
            t = glm::cross(b,n);

            t = glm::normalize(t);

            vertex_data[i].tangent = glm::vec4(t.x,t.y,t.z,1.0f);            
        }
    }
	
protected:
	vector<VertexAttribs> vertex_data;
	vector<unsigned int> triangles, triangle_texture_indices,triangle_normal_indices;

	glm::vec4 minBounds,maxBounds; //bounding box


};

#endif