#pragma once
#include "GraphicObject.h"
#include "HitBox.h"
#include "Light.h"

#include <glm/gtx/string_cast.hpp>
#include <map>
#include <string>

class GameObject
{
public:
	GameObject(glm::vec3 pos, GraphicObject* graphicObject);

	std::string GetName();
	GraphicObject* GetHitboxGraphic();
	GraphicObject* GetGraphics();
	glm::vec3 GetHitBoxMin();
	glm::vec3 GetHitBoxMax();
	HitBox* GetHitBox();
	const glm::vec3 GetPosition();
	Light* GetObjectLight();
	glm::mat4 GetRotMatrix();
	bool IsHitboxOverridden() { return overrideHitboxScale; }

	void SetName(std::string n);
	void SetHitBox(glm::vec3 min, glm::vec3 max);
	void SetScale(glm::vec3 s);
	void SetPosition(glm::vec3 p);
	void SetCollision(bool t);
	void SetObjectLight(Light* l);
	void ScaleHitbox(glm::vec3 s, bool override = false);
	void SetRotation(glm::vec3 direction, float amount);
	void SetHitboxOverride(bool b) { overrideHitboxScale = b; }
	void SetNoCollide() { collide = false; }
	
	bool IsCollider();
	bool IsLight();

private:
	float Yaw = 0.0f;
	float Pitch = 0.0f;
	float Roll = 0.0f;
	bool overrideHitboxScale = false;
	std::string name;
	glm::mat4 RotMatrix;
	GraphicObject* hitboxGraphic;
	GraphicObject* graphics;
	glm::vec3 position;
	HitBox* hitbox;
	bool collide = true;
	Light* ObjectLight = nullptr;
};

