#include "Light.h"

Light::Light(glm::vec3 pos, glm::vec3 col, float intens)
{
	EditHitbox = HitBox(glm::vec3(-.25, -.25, -.25), glm::vec3(.25, .25, .25));
	position = pos;
	color = col;
	intensity = intens;
}

glm::vec3 Light::GetPosition()
{
	return position;
}

void Light::SetPosition(glm::vec3 pos)
{
	position = pos;
}

glm::vec3 Light::GetColor()
{
	return color;
}

float Light::GetIntensity()
{
	return intensity;
}

void Light::SetColor(glm::vec3 c)
{
	color = c;
}
