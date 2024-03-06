#pragma once
#include "Button.h"
#include "GameObject.h"
#include "GraphicObject.h"
#include "Light.h"
#include "LightInfo.h"
#include "QuadGraphic.h"
#include "Text.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <map>


class Graphics
{
public:
	const int SCREEN_WIDTH = 1920;
	const int SCREEN_HEIGHT = 1080;
	Graphics(bool isEdit);
	~Graphics();
	void PostMapLoadUpdate() { SetLightRenderInfo(); }
	void RenderObjects(std::vector<GraphicObject*>* graphicObjects, std::vector<Text*> textObjects, std::vector<Button*> buttonObjects, std::vector<QuadGraphic*> quadGraphics, glm::mat4 ProjMatrix, glm::mat4 ViewMatrix);
	void RenderGraphic(GraphicObject* graphic, glm::mat4 Projection, glm::mat4 View);
	GLFWwindow* GetWindow();
	int GraphicsInit(bool isEdit);
	GraphicObject* CreateGraphicObject(const char* vertpath, const char* fragpath, const char* modelpath);
	GraphicObject* CreateGraphicObject(const char* modelpath);
	void DeleteGraphicObject(GraphicObject* o);
	glm::vec2 GetScreenSize();
	std::vector<GraphicObject*>* GetGraphicObjects();
	Mesh* CreateMesh(const char* modelPath);
	Shader* CreateShader(const char* vPath, const char* fPath);
	void SetModel(GraphicObject* object, const char* path);
	std::vector<Light*>* GetLights();
	Light* CreateLight(GameObject* o, glm::vec3 col, float intens);
	void AddGraphicObject(GraphicObject* graphic);

	void LineInit(glm::vec3 position, glm::vec3 vector, float scale);
	void RenderLine(glm::mat4 projection, glm::mat4 view);

	void ChangeLine(glm::vec3 position, glm::vec3 vector, float scale);
	GLuint CreateTexture(std::string path);


private:
	const int LIGHT_MAX = 16;
	const char* LIGHT_VERTEX_SHADER_PATH = "shaders/LightBoxVertexShader.vertexshader";
	const char* LIGHT_FRAGMENT_SHADER_PATH = "shaders/LightBoxFragmentShader.fragmentshader";
	GLuint LoadDDS(const char* path);
	void SetLightRenderInfo();
	std::map<std::string, GLuint> mTextureMap;
	std::map<std::string, Mesh*> mModelMap;
	std::map<const char*, Shader*> mShaderMap;
	std::vector<Light*>* Lights = new std::vector<Light*>;

	LightInfo LightRenderInfo;
	std::vector<GraphicObject*>* GraphicObjects;
	GLFWwindow* pWindow;

	GLuint LVertexArrayID;
	GLuint LVertexBuffer;
	Shader LShader;
};