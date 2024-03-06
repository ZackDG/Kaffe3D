#pragma once
#include <glm/gtx/transform.hpp>

class HitBox
{
public:
	HitBox(glm::vec3 min, glm::vec3 max) {
		Min = min;
		Max = max;
		offset = glm::vec3(0);
		scale = glm::vec3(1);

		if (min != max)
		{
			points[0] = glm::vec3(min.x, min.y, min.z);
			points[1] = glm::vec3(min.x, min.y, max.z);
			points[2] = glm::vec3(min.x, max.y, min.z);
			points[3] = glm::vec3(min.x, max.y, max.z);
			points[4] = glm::vec3(max.x, min.y, min.z);
			points[5] = glm::vec3(max.x, min.y, max.z);
			points[6] = glm::vec3(max.x, max.y, min.z);
			points[7] = glm::vec3(max.x, max.y, max.z);

			glm::vec3 temp_faces[6][3] = {
			{points[3], points[1], points[0]},
			{ points[1], points[5], points[7]},
			{ points[5], points[4], points[6]},
			{ points[2], points[0], points[4]},
			{ points[7], points[6], points[2]},
			{ points[5], points[1], points[0]}
			};
			for (int i = 0; i < 6; i++)
			{
				for (int a = 0; a < 3; a++)
				{
					faces[i][a] = temp_faces[i][a];
				}
			}

			for (int i = 0; i < 6; i++)
			{
				normals[i] = CreateNormal(faces[i][0], faces[i][1], faces[i][2]);
			}
			normals[0] = normals[0] * -1.0f;
			normals[3] = normals[3] * -1.0f;
		}

		glm::vec3 normA(0.0);
		glm::vec3 normB(0.0);
		glm::vec3 normC(0.0);
		//glm::vec3 normBA(0.0);
		//glm::vec3 normBC(0.0);

		//X Normals
		normA = glm::vec3(min.x, min.y, min.z);
		normB = glm::vec3(min.x, max.y, min.z);
		normC = glm::vec3(min.x, max.y, max.z);
		xMaxNormal = -CalculateCrossProduct(normA, normB, normC);
		xMinNormal = -xMaxNormal;

		//Y Normals
		normA = glm::vec3(min.x, min.y, min.z);
		normB = glm::vec3(min.x, min.y, max.z);
		normC = glm::vec3(max.x, min.y, max.z);
		yMaxNormal = CalculateCrossProduct(normA, normB, normC);
		yMinNormal = -yMaxNormal;

		//Z Normals
		normA = glm::vec3(min.x, min.y, min.z);
		normB = glm::vec3(max.x, min.y, min.z);
		normC = glm::vec3(max.x, max.y, min.z);
		zMaxNormal = -CalculateCrossProduct(normA, normB, normC);
		zMinNormal = -zMaxNormal;
		// Set Edges.
		edges[0] = glm::vec3(max.x, min.y, min.z) - min;
		edges[1] = glm::vec3(min.x, max.y, min.z) - min;
		edges[2] = glm::vec3(min.x, min.y, max.z) - min;
	}
	HitBox() {
		Min = glm::vec3(0, 0, 0);
		Max = Min;
	}

	glm::vec3 CalculateCrossProduct(glm::vec3 normA, glm::vec3 normB, glm::vec3 normC)
	{
		glm::vec3 normBA = glm::vec3(normA.x - normB.x, normA.y - normB.y, normA.z - normB.z);
		glm::vec3 normBC = glm::vec3(normC.x - normB.x, normC.y - normB.y, normC.z - normB.z);
		glm::vec3 cross = glm::cross(normBC, normBA);
		return glm::normalize(cross);
	}

	void SetOffset(glm::vec3 o)
	{
		offset = o;
	}



	void Rotate(glm::mat4 rotMatrix)
	{
		RotatePoints(rotMatrix);
		RotateNormals(rotMatrix);
		RotateFaces(rotMatrix);
		RotateEdges(rotMatrix);
	}

	glm::vec3* GetNormals()
	{
		return normals;
	}
	glm::vec3 GetNormal(int i)
	{
		return normals[i];
	}

	glm::vec3 GetFacePoint(int i)
	{
		return faces[i][0];
	}

	glm::vec3 GetFacePoint(glm::vec3 norm)
	{
		for (int i = 0; i < 6; i++)
		{
			if (norm == normals[i])
			{
				return faces[i][0];
			}
		}
	}

	bool IsPointInAxis(glm::vec3 point, int i)
	{
		// check that intersection is within bounds of face

// Create two axes for each face to test boundaries

		glm::vec3 axis1 = faces[i][1] - faces[i][0];
		glm::vec3 axis2 = faces[i][2] - faces[i][1];

		float axes1Points[2] = { 0 };
		axes1Points[0] = glm::dot(axis1, faces[i][0]);
		axes1Points[1] = glm::dot(axis1, faces[i][1]);

		float axes2Points[2] = { 0 };
		axes2Points[0] = glm::dot(axis2, faces[i][1]);
		axes2Points[1] = glm::dot(axis2, faces[i][2]);

		float axes1Min = axes1Points[0];
		float axes1Max = axes1Points[1];
		if (axes1Min > axes1Max)
		{
			axes1Min = axes1Points[1];
			axes1Max = axes1Points[0];
		}

		float axes2Min = axes2Points[0];
		float axes2Max = axes2Points[1];
		if (axes2Min > axes2Max)
		{
			axes2Min = axes2Points[1];
			axes2Max = axes2Points[0];
		}

		// Project our intersect onto each axis and see if its within the bounds.
		float pointAxis1 = glm::dot(axis1, point);
		float pointAxis2 = glm::dot(axis2, point);
		
		float epsilon = 10e-5;

		if ((pointAxis1 > axes1Min - epsilon && pointAxis1 < axes1Max + epsilon) &&
			(pointAxis2 > axes2Min - epsilon && pointAxis2 < axes2Max + epsilon))
		{
			// If we are going in same direction as normal, don't care about the collision.
			// dot product should be negative if our direction opposes normals direction.
			//float sign = glm::dot(normals[i], std::get<1>(intersectData));
			//// Sign bit returns true if negative
			//if (signbit(sign) == true)
			//{
			//	return normals[i];
			//}
			return true;
		}
	}

	glm::vec3 points[8] = { glm::vec3(0) };
	glm::vec3 faces[6][3];
	glm::vec3 edges[3];


	glm::vec3 Min;
	glm::vec3 Max;


	//hitbox center offset from gameobject position
	glm::vec3 offset;
	glm::vec3 scale;

	glm::vec3 xMinNormal;
	glm::vec3 xMaxNormal;
	glm::vec3 yMinNormal;
	glm::vec3 yMaxNormal;
	glm::vec3 zMinNormal;
	glm::vec3 zMaxNormal;

private:
	glm::vec3 normals[6] = { glm::vec3(0) };

	void RotatePoints(glm::mat4 rotMatrix)
	{
		for (int i = 0; i < 8; i++)
		{
			glm::vec4 temp = glm::vec4(points[i].x, points[i].y, points[i].z, 1);
			points[i] = rotMatrix * temp;
		}
	}
	void RotateFaces(glm::mat4 rotMatrix)
	{
		for (int i = 0; i < 6; i++)
		{
			for (int a = 0; a < 3; a++)
			{
				glm::vec4 temp = glm::vec4(faces[i][a].x, faces[i][a].y, faces[i][a].z, 1);
				faces[i][a] = rotMatrix * temp;
			 }

		}
	}

	void RotateNormals(glm::mat4 rotMatrix)
	{
		for (int i = 0; i < 6; i++)
		{
			glm::vec4 temp = glm::vec4(normals[i].x, normals[i].y, normals[i].z, 1);
			normals[i] = rotMatrix * temp;
		}
	}

	void RotateEdges(glm::mat4 rotMatrix)
	{
		edges[0] = points[4] - points[0];
		edges[1] = points[2] - points[0];
		edges[2] = points[1] - points[0];
	}

	glm::vec3 CreateNormal(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC)
	{
		glm::vec3 edge1 = pointB - pointA;
		glm::vec3 edge2 = pointC - pointA;
		glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

		return normal;
	}
};

