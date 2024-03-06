#pragma once
#include "Timer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

class Controls
{
public:
	Controls(GLFWwindow* pwind, glm::vec3 position);
	~Controls();
	void UpdateCameraAngle(double deltaTime, bool isEdit);
	void EditModeMouseUpdate(double deltaTime);
	void SetViewMatrix(glm::vec3 position);
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();
	glm::vec3 GetLookDirection();
	glm::vec3 GetPlayersRight();

	float GetDeltaHAngle();
	float GetDeltaVAngle();

	

private:
	GLFWwindow* pWindow;
	glm::vec2 windowDimensions;
	int width;
	int height;
	glm::vec3 direction;
	glm::vec3 right;
	glm::vec3 up;

	// horizontal angle : toward -Z
	float horizontalAngle;
	// vertical angle : 0, look at the horizon
	float verticalAngle;

	float deltaHAngle;
	float deltaVAngle;

	double lastXPos = 0.0f;
	double lastYPos = 0.0f;

	float mouseHAngle;
	float mouseVangle;
	// Initial Field of View
	float initialFoV = 90.0f;

	//float speed = 3.0f; // 3 units / second
	float mouseSpeed = 0.4f;

	double lastTime = 0.0f;

	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;

	float radius = 5.0f;
	glm::vec3 center = glm::vec3(0, 0, 0);

};

