#pragma once
#include "TestShader.h"
#include "Mesh.h"
#include "Light.h"
#include "LightInfo.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <iterator>
#include <stdlib.h>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

class GraphicObject
{
public:
	GraphicObject(const char* vShaderPath, const char* fShaderPath, const char* modelPath);
	GraphicObject(const char* modelPath) : GraphicObject("shaders/SimpleVertexShader.vertexshader", "shaders/SimpleFragmentShader.fragmentshader", modelPath) {};
	GraphicObject() : GraphicObject("shaders/SimpleVertexShader.vertexshader", "shaders/SimpleFragmentShader.fragmentshader", nullptr) {};

	void RenderObject(glm::mat4& Projection, glm::mat4& View, LightInfo lightInfo);
	void Rotate(glm::vec3 direction, float angle);
	std::string ModelMatString();
	void InitialScale(glm::vec3 s);

	void SetPosition(glm::vec3 pos);
	void SetScale(glm::vec3 s);
	void SetTexture(GLuint textureID, const char* path);
	void SetWireFrame();
	void SetNewShader(GLuint id);
	void SetShaderID(GLuint id);
	void SetModelMatrix(glm::mat4 matrix);
	void SetLight(bool t);
	void SetMeshPointer(Mesh* mesh);
	void SetModelPath(const char* path) { ModelPath = path; }
	void Transform(glm::vec3 pos, glm::vec3 rotationAxis, float angle, glm::vec3 scale);

	GLuint GetShaderID();
	const char* GetModelPath();
	const char* GetVShader();
	const char* GetFShader();
	glm::mat4 GetModelMatrix();
	glm::vec3 GetScale();
	const char* GetTexture();
	glm::vec3 GetRotationAxis() { return rotationAxis; }
	float GetAngle() { return Angle; }

	bool IsLight();

private:
	void ScaleSetPos(glm::vec3 pos);
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotationAxis = glm::vec3(0,1,0);
	bool WireFrame = false;
	std::string ModelPath;
	std::string TexturePath;
	const char* VShaderPath;
	const char* FShaderPath;
	bool light = false;
	float Angle = 0.0f;
	glm::mat4 ModelMatrix;
	GLuint ModelMatrixID;
	GLuint MVPMatrixID;
	GLuint VMatrixID;
	GLuint TextureID;
	GLuint LightColorID;
	GLuint LightPosID;
	GLuint LightPowerID;
	GLuint ShaderID;
	Mesh* ObjectMesh;
	Mesh* HitboxMesh;
};

