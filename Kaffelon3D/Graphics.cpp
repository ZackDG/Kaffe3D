#include "Graphics.h"

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII
Graphics::Graphics(bool isEdit)
{
	GraphicObjects = new std::vector<GraphicObject*>;
	GraphicsInit(isEdit);
	LightRenderInfo = LightInfo();

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	// Hide cursor when in play mode
	if (!isEdit)
		glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(pWindow, GLFW_STICKY_KEYS, GL_TRUE);
}

Graphics::~Graphics()
{
}

struct LightRenderInfo {
	glm::vec3 positions[16];
	glm::vec3 colors[16];
	float intensities[16];
};

void Graphics::RenderObjects(std::vector<GraphicObject*>* graphicObjects, std::vector<Text*> textObjects, std::vector<Button*> buttonObjects, std::vector<QuadGraphic*> quadGraphics, glm::mat4 ProjMatrix, glm::mat4 ViewMatrix)
{
	// Clear the rendering buffers then draw
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//Render 3D Objects
	for (GraphicObject* it : *graphicObjects)
	{
		if (!it->IsLight())
			it->RenderObject(ProjMatrix, ViewMatrix, LightRenderInfo);
	}
	// Render UI
	for (QuadGraphic* quad : quadGraphics)
		quad->Render();
	for (Text* it : textObjects)
		it->Render();
	for (Button* it : buttonObjects)
		it->Render();
}

void Graphics::RenderGraphic(GraphicObject* graphic, glm::mat4 Projection, glm::mat4 View)
{
	graphic->RenderObject(Projection, View, LightRenderInfo);
}

GLFWwindow* Graphics::GetWindow()
{
	return pWindow;
}


int Graphics::GraphicsInit(bool isEdit)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	pWindow = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Kaffe3D", NULL, NULL);

	if (pWindow == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(pWindow);
	//Unlimits the frame rate for testing purposes.
	glfwSwapInterval(0);


	glewExperimental = GL_TRUE;
	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}



	//glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glClearColor(0.1f, 0.0f, 0.02f, 0.0f);

	return 0;
}

GraphicObject* Graphics::CreateGraphicObject(const char* vertpath, const char* fragpath, const char* modelpath)
{
	GraphicObject* newGraphicObject = new GraphicObject(modelpath);
	GraphicObjects->push_back(newGraphicObject);

	newGraphicObject->SetMeshPointer(CreateMesh(modelpath));
	newGraphicObject->SetModelPath(modelpath);
	Shader* newShader = CreateShader(vertpath, fragpath);
	newGraphicObject->SetNewShader(newShader->ID);

	return newGraphicObject;
}

GraphicObject* Graphics::CreateGraphicObject(const char* modelpath)
{
	GraphicObject* newGraphicObject = CreateGraphicObject("shaders/SimpleVertexShader", "shaders/SimpleFragmentShader", modelpath);
	return newGraphicObject;
}

void Graphics::DeleteGraphicObject(GraphicObject* o)
{
	std::vector<GraphicObject*>::iterator it = std::find(GraphicObjects->begin(), GraphicObjects->end(), o);
	GraphicObjects->erase(it);
}

glm::vec2 Graphics::GetScreenSize()
{
	return glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT);
}

std::vector<GraphicObject*>* Graphics::GetGraphicObjects()
{
	return GraphicObjects;
}


Mesh* Graphics::CreateMesh(const char* modelPath)
{
	std::string path = modelPath;
	if (mModelMap.find(path) == mModelMap.end())
	{
		Mesh* newMesh = new Mesh();
		newMesh->ImportFromPath(modelPath);
		mModelMap.insert(std::pair<std::string, Mesh*>(path, newMesh));
	}
	return mModelMap[path];
}

Shader* Graphics::CreateShader(const char* vPath, const char* fPath)
{
	if (mShaderMap.find(vPath) == mShaderMap.end())
	{
		Shader* newShader = new Shader(vPath, fPath);
		mShaderMap.insert(std::pair<const char*, Shader*>(vPath, newShader));
	}
	return mShaderMap[vPath];
}

void Graphics::SetModel(GraphicObject* object, const char* path)
{
	object->SetMeshPointer(CreateMesh(path));
	object->SetModelPath(path);
	
}

std::vector<Light*>* Graphics::GetLights()
{
	return Lights;
}

Light* Graphics::CreateLight(GameObject* o, glm::vec3 col, float intens)
{
	Light* newLight = new Light(o->GetPosition(), col, intens);
	Lights->push_back(newLight);
	o->SetObjectLight(newLight);
	o->GetGraphics()->SetLight(true);

	Shader* newShader = CreateShader(LIGHT_VERTEX_SHADER_PATH, LIGHT_FRAGMENT_SHADER_PATH);
	o->GetHitboxGraphic()->SetNewShader(newShader->ID);
	return newLight;
}

void Graphics::AddGraphicObject(GraphicObject* graphic)
{
	if (graphic == nullptr)
	{
		std::cout << "Adding null graphic object! " << "\n";
		return;
	}
	GraphicObjects->push_back(graphic);
}

void Graphics::LineInit(glm::vec3 position, glm::vec3 vector, float scale)
{
	LVertexArrayID = 0;
	LVertexBuffer = 0;
	glGenVertexArrays(1, &LVertexArrayID);
	glBindVertexArray(LVertexArrayID);

	GLfloat g_vertex_buffer_data[] = {
		position.x, position.y, position.z,
		position.x, position.y, position.z,
		position.x + (vector.x * scale), position.y + (vector.y * scale), position.z + (vector.z * scale)
	};
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &LVertexBuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, LVertexBuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	LShader = Shader("shaders/WireVertexShader.vertexshader", "shaders/WireFragmentShader.fragmentshader");
}



void Graphics::RenderLine(glm::mat4 projection, glm::mat4 view)
{
	glm::mat4 ModelMatrix = glm::mat4(1);
	glm::mat4 MVP = projection * view * ModelMatrix;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);

	glUseProgram(LShader.ID);
	GLuint MVPMatrixID = glGetUniformLocation(LShader.ID, "MVP");

	glUniformMatrix4fv(MVPMatrixID, 1, GL_FALSE, &MVP[0][0]);
	glBindVertexArray(LVertexArrayID);

	// set up buffers and attributes.
	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, LVertexBuffer);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
	//ButtonText->Render();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
}

void Graphics::ChangeLine(glm::vec3 position, glm::vec3 vector, float scale)
{
	glBindBuffer(GL_ARRAY_BUFFER, LVertexBuffer);

	GLfloat g_vertex_buffer_data[] = {
	position.x, position.y, position.z,
	position.x, position.y, position.z,
	position.x + (vector.x * scale), position.y + (vector.y * scale), position.z + (vector.z * scale)
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
}

GLuint Graphics::CreateTexture(std::string path)
{
	if (mTextureMap.find(path) == mTextureMap.end()) {
		GLuint textureID = LoadDDS(path.c_str());
		mTextureMap.insert(std::pair<std::string, GLuint>(path, textureID));
		return textureID;
	}
	else {
		return mTextureMap[path];
	}
}
// From opengl-tutorial.org
GLuint Graphics::LoadDDS(const char* path)
{
	unsigned char header[124];

	//FILE* fp;
	FILE* file;
	errno_t err;

	err = fopen_s(&file, path, "rb");
	/* try to open the file */
	//fp = fopen_s(path, "rb");
	if (err != 0)
		return 0;

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, file);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(file);
		return 0;
	}

	/* get the surface desc */
	fread(&header, 124, 1, file);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);

	unsigned char* buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, file);
	/* close the file pointer */
	fclose(file);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return 0;
	}

	// Create one OpenGL texture
	//GLuint textureID;
	GLuint TextureID;
	glGenTextures(1, &TextureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, TextureID);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */
	if (mipMapCount > 1)
	{
		for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
		{
			unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
			glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
				0, size, buffer + offset);

			offset += size;
			width /= 2;
			height /= 2;
		}
	}
	// If dds doesn't have mip maps, generate our own.
	else
	{
		unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, format, width, height,
			0, size, buffer + offset);

		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 3);
	}

	free(buffer);

	return TextureID;
}

void Graphics::SetLightRenderInfo()
{
	glm::vec3 positions[16];
	glm::vec3 colors[16];
	float intensities[16];

	for (int i = 0; i < Lights->size(); i++)
	{
		if (i > LIGHT_MAX)
		{
			std::cout << "Too many Lights!" << "\n";
		}
		positions[i] = Lights->at(i)->GetPosition();
		colors[i] = Lights->at(i)->GetColor();
		intensities[i] = Lights->at(i)->GetIntensity();
	}
	LightRenderInfo = LightInfo(positions, colors, intensities);
}

