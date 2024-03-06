#pragma once
#include "GameObject.h"
#include "Graphics.h"
#include "Physics.h"
#include "Player.h"
#include "World.h"

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <future>
#include <glm/gtx/string_cast.hpp>
#include <string>

class EditorControls
{
public:
	EditorControls(World* w);
	~EditorControls();
	void ClickObject(std::vector<GameObject*>* objects, Graphics* graphics, Player* player, Timer* timer);
	GameObject* GetClickedObject();
	void GameObjectEditControls(GLFWwindow* window);
	void MoveClickedObject(GLFWwindow* window, Player* player, float deltaTime);
	void ScaleClickedObject(GLFWwindow* window, Player* player, float deltaTime);
	void CheckConsoleInput();
	static std::string GetConsoleCommand();
private:
	bool duplicatePressed = false;
	GameObject* clickedObject = nullptr;
	GameObject* hoveredObject = nullptr;
	Shader editShader = Shader("shaders/editVShader.vertexshader", "shaders/editFShader.fragmentshader");
	GLuint editSID = editShader.ID;
	GLuint prevSID;
	bool clicked = false;
	float moveSpeed = 15000;
	float scaleSpeed = 5000;

	// 0 = left, 1 = right,
	// 2 = up, 3 = down, 
	// 4 = z-  5 = z+
	bool Pressed[6] = { false };
	std::future<std::string> ConsoleInput;

	World* world;
	std::string command = "";
};

