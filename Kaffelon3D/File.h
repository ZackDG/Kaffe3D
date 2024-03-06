#pragma once
#include <iostream>
#include "GameObject.h";
#include <string>
#include "glm/glm.hpp"
#include "World.h"
#include <glm/gtx/string_cast.hpp>


class File
{
public:
	static void SaveObjects(std::vector<GameObject*>* objects, const char* outputPath);
	static void LoadObjects(const char* filepath, World* world);

private:
	static void SaveObject(GameObject* o, std::ofstream* outfile);
	static void GetVector3FromString(std::string line, std::string* xSub, std::string* ySub, std::string* zSub);
	static std::string GetVector4FromString(std::string line, std::string* x, std::string* y, std::string* z, std::string* w);
	static glm::mat4 GetMatrix4FromString(std::string line);
	struct save_data;
};

