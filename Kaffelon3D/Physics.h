#pragma once
#include "GameObject.h"
#include "Timer.h"
#include "Player.h"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/string_cast.hpp"
#include <vector>
#include <tuple>
#include <iostream>
#include <Windows.h>
#include <map>
#include <set>

class Physics
{
public:
	Physics(Timer* mainTimer);
	glm::vec3 PlayerConvexCollision(std::vector<GameObject*>* objects, Player* player, bool last);
	static GameObject* MouseOverObjects(std::vector<GameObject*>* objects, Player* player);
	static std::tuple<bool, glm::vec3> IntersectObjectFace(char face, GameObject* object, glm::vec3 lineStart, glm::vec3 lineEnd);
	float GetGravity();
	static std::tuple<bool, glm::vec3>  LineSegIntersectPlane(glm::vec3 p0, glm::vec3 p1, glm::vec3 normal, glm::vec3 planePoint);
	void Update(std::vector<GameObject*>*, Player*, bool, Timer* mainTimer);
private:
	static void AssignNewClosest(std::tuple<GameObject*, float>* Closest, glm::vec3 lineStart, glm::vec3 intersect, GameObject* object);
	int MaxCollisions = 4;
	float MaxWalkAngle = 45.0f;
	float gravity = -10.0f;
	float deltaTime = 0.0f;
	double physicsStepDelta = .02;
	std::chrono::time_point<std::chrono::system_clock> lastTimeStep;	
};

