#pragma once
#include <string>
#include <string>
#include <functional>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "TestShader.H"
#include "Text.h"


class QuadGraphic
{
public:
	void UpdateMesh(float width, float height, float screenWidth, float screenHeight);
	QuadGraphic(std::string text, glm::vec2 startPosition, float width, float height, glm::vec4 color, float screenWidth, float screenHeight, GLFWwindow* win, std::function<void()> function);
	void Render();
private:
	glm::vec2 Position;
	std::string ButtonString;
	bool Clicked = false;
	std::function<void()> ButtonFunction;
	GLFWwindow* window;
	glm::vec2 RatioPosition;
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

