#pragma once
#include <glm/glm.hpp>

class LightInfo
{
public:
	LightInfo(glm::vec3 pos[16], glm::vec3 col[16], float intens[16])
	{
		for (int i = 0; i < 16; i++)
		{
			positions[i] = pos[i];
			colors[i] = col[i];
			intensities[i] = intens[i];
		}
	};
	LightInfo(){};
	glm::vec3 positions[16] = { glm::vec3(0) };
	glm::vec3 colors[16] = { glm::vec3(0) };
	float intensities[16] = { 0.0f };
};

