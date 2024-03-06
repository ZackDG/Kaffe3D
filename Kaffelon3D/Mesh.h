#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>


class Mesh
{
public:
	Mesh();
	~Mesh();
	void PrepareMesh();
	void Finished();
	int GetVerticesCount();
	bool ImportFromPath(const std::string& pFile);
private:
	void SetupBuffers();
	std::vector<unsigned short> Indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	GLuint VertexArray;
	GLuint VertexBuffer;
	GLuint UVBuffer;
	GLuint NormalBuffer;
	GLuint ElementBuffer;
};

