#pragma once

#include "Mesh.h"
#include "TestShader.H"
#include "Text.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <iostream>


class Button
{
public:
	void Render();
	Button(std::string text, glm::vec2 position, float width, float height, glm::vec4 color, float screenWidth, float screenHeight, GLFWwindow* win, std::function<void()> function);
	bool IsHover();
private:
	std::string ButtonString;
	bool Clicked = false;
	std::function<void()> ButtonFunction;
	GLFWwindow* pWindow;
	glm::vec2 NormalizedPosition;
	glm::vec2 PixelPosition;
	glm::vec2 PixelDimension;
	float ScreenWidth;
	float ScreenHeight;
	Text* ButtonText;
	float ButtonColor[4] = { 0,0,0,1 };
	GLuint ColorID;
	GLuint VertexArrayID;
	GLuint VertexBuffer;
	Shader ButtonShader;
};

