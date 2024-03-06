#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <GL/glew.h>
#include "TestShader.H"
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

class Text
{
public:
	Text(glm::vec3 color, glm::vec2 position, std::string text, int size, float screenWidth, float screenHeight);
	void Render();
	void SetText(const char* string);
private:
	const char* VERTEX_SHADER_PATH = "shaders/textVShader.vertexshader";
	const char* FRAGMENT_SHADER_PATH = "shaders/textFShader.fragmentshader";
	struct Character {
		unsigned int TextureID;  // ID handle of the glyph texture
		glm::ivec2   Size;       // Size of glyph
		glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
		unsigned int Advance;    // Offset to advance to next glyph
	};
	bool Init(int size);
	void Prepare();
	void Finished();
	glm::vec3 Color;
	glm::vec2 Position;
	FT_Face face;
	FT_Library ft;
	glm::mat4 TextProjection;
	Shader TextShader;
	unsigned int Texture;
	unsigned int VertexArray;
	unsigned int VertexBuffer;
	std::map<char, Character> Characters;
	std::string TextString;
	float Scale = 1.0f;
};

