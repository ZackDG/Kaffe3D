#include "Text.h"


Text::Text(glm::vec3 color, glm::vec2 position, std::string text,int size, float screenWidth, float screenHeight)
{
	VertexBuffer = 0;
	VertexArray = 0;
	TextShader.ShaderInit(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	Color = color;
	Position = glm::vec2(position.x * screenWidth, position.y* screenHeight) / 100.0f;
	TextString = text;
	TextProjection = glm::ortho(0.0f, screenWidth, 0.0f, screenHeight);
	Texture = 0;
	glGenVertexArrays(1, &VertexArray);
	glGenBuffers(1, &VertexBuffer);

	Init(size);
}
void Text::Render()
{
	//Turn on transparency(needed for text textures)
	glEnable(GL_BLEND);
	// activate corresponding render state	
	TextShader.Use();
	Prepare();
	glBindVertexArray(VertexArray);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glActiveTexture(GL_TEXTURE0);
	glUniform3f(glGetUniformLocation(TextShader.ID, "textColor"), Color.x, Color.y, Color.z);
	glUniformMatrix4fv(glGetUniformLocation(TextShader.ID, "projection"), 1, GL_FALSE, &TextProjection[0][0]);

	float originalX = Position.x;
	// iterate through all characters
	std::string::const_iterator c;
	for (c = TextString.begin(); c != TextString.end(); c++)
	{
		Character ch = Characters[*c];

		float xpos = Position.x + ch.Bearing.x * Scale;
		float ypos = Position.y - (ch.Size.y - ch.Bearing.y) * Scale;

		float w = ch.Size.x * Scale;
		float h = ch.Size.y * Scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		Position.x += (ch.Advance >> 6) * Scale; // bitshift by 6 to get value in pixels (2^6 = 64)
	}
	Position.x = originalX;
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	Finished();
	//Turn off transparency.
	glDisable(GL_BLEND);
}

void Text::SetText(const char* string)
{
	TextString = string;
}

bool Text::Init(int size)
{


	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return -1;
	}

	if (FT_New_Face(ft, "CALIBRI.ttf", 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return -1;
	}

	FT_Set_Pixel_Sizes(face, 0, size);

	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
	{
		std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
		return -1;
	}


	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

	for (unsigned char c = 0; c < 128; c++)
	{
		// load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// generate texture
		glGenTextures(1, &Texture);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// now store character for later use
		Character character = {
			Texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<char, Character>(c, character));
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void Text::Prepare()
{
	glBindVertexArray(VertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Text::Finished()
{
	glDisableVertexAttribArray(0);
}
