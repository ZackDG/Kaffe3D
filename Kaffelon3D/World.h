#pragma once
#include "Graphics.h"
#include "Controls.h"
#include "GameObject.h"
#include <vector>
#include "Text.h"
#include <string>
#include "glm/glm.hpp"

class World
{
public:
	World(bool isEdit);
	GameObject* CreateObject(glm::vec3 pos, const char* modelPath);
	GameObject* CreateObject(glm::vec3 pos, const char* modelPath, glm::vec3 hitboxmin, glm::vec3 hitboxmax);
	GameObject* CreateObject(glm::vec3 pos, const char* modelPath, glm::vec3 hitboxmin, glm::vec3 hitboxmax, const char* vshader, const char* fshader, glm::mat4 modelMatrix, glm::vec3 scale);
	void DuplicateObject(GameObject* o);
	void DeleteObject(GameObject* o);

	Text CreateText(glm::vec3 color, glm::vec2 pos, std::string text, int size);
	GLFWwindow* GetWindow();
	std::vector<GameObject*>* GetGameObjects();
	Graphics* GetGraphics();
	void RenderHitBoxes(glm::mat4 Projection, glm::mat4 View);
	void HitboxInit(GameObject* object);
	void SetPosition(glm::vec3* objectPos, glm::vec3 newPos);
	GameObject* CreateLight(glm::vec3 pos, glm::vec3 col, float intens);
private:
	Graphics* graphics;
	std::vector<GameObject*>* GameObjects;
	//GameObject* GameObject1;
	std::vector<Text*> TextObjects;
	// True = Edit, False = Play
	bool edit = false;
};

