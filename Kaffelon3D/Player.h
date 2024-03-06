#pragma once
#include "Controls.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Timer.h"
#include <iostream>
class Player
{
public:
	Player(GLFWwindow* window);
	void UpdateFromInputs(double deltaTime, bool edit);
 	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();
	glm::vec3 GetDesiredPosition();
	glm::vec3 GetHitBoxMin();
	glm::vec3 GetHitBoxMax();
	glm::vec3 GetFeetMin();
	glm::vec3 GetFeetMax();
	glm::vec3 GetPosition();
	glm::vec3 GetVelocity();
	Controls* GetControls();
	void SetOnGround();
	bool IsOnGround();
	void PlayerInAir();
	void SetPlayerCeiling(bool t);
	bool isCeiling();
	void Jump(double deltaTime);
	void adjustVelocity(glm::vec3 adjustment);
	void SetXZVelocity(glm::vec2 adjustment);
	void SetYVelocity(float y);
	void MoveToDesired();
	void SetDesiredPos(glm::vec3 pos);
	void ApplyPlayerGravity(double deltaTime);
	void SetOnCeiling();
	void TakeDamage(float damage);
	float GetHP();
	glm::vec3* GetAxes();
	glm::vec3* GetEdges();
	glm::vec3 GetPoint(int i);
private:
	void SetNextPoints();
	float health = 100.0f;
	float inputSpeed = 10.0f;
	float editInputSpeed = 25.0f;
	float jumpHeight = 7.0f;
	float gravity = -50.0f;
	bool onGround = false;
	bool onCeiling = false;
	glm::vec3 velocity;
	glm::vec3 feetMin;
	glm::vec3 feetMax;
	glm::vec3 hitBoxMin;
	glm::vec3 hitBoxMax;
	glm::vec3 axes[3];
	glm::vec3 edges[3];
	Controls* controls;
	glm::vec3 position = glm::vec3(0, -5, 5);
	glm::vec3 desiredPosition = position;
	GLFWwindow* window;
	glm::vec3 NextPoints[8];
};

