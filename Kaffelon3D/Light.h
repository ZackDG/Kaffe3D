#pragma once
#include <glm/glm.hpp>
#include "HitBox.h"

class Light
{
public:
	Light(glm::vec3 pos, glm::vec3 col, float intens);
	glm::vec3 GetPosition();
	void SetPosition(glm::vec3 pos);
	glm::vec3 GetColor();
	float GetIntensity();
	void SetIntensity(float i) { intensity = i; }
	void SetColor(glm::vec3 c);
private:
	glm::vec3 position;
	glm::vec3 color;
	float intensity = 10.0f;
	HitBox EditHitbox;
};

