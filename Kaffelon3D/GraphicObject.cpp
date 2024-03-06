#include "GraphicObject.h"

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GraphicObject::GraphicObject(const char* vShaderPath, const char* fShaderPath, const char* modelPath)
{
	ModelPath = modelPath;
	VShaderPath = vShaderPath;
	FShaderPath = fShaderPath;
	position = glm::vec3(0);
	scale = glm::vec3(1);
	glm::mat4 ModelMatrix = glm::mat4(1.0f);
}

void GraphicObject::SetPosition(glm::vec3 pos)
{
	position = pos;
	Transform(position, rotationAxis, Angle, scale);
}

void GraphicObject::SetScale(glm::vec3 s)
{
	scale = s;
	Transform(position, rotationAxis, Angle, scale);
}

void GraphicObject::RenderObject(glm::mat4 &Projection, glm::mat4 &View, LightInfo lightInfo)
{
	glm::mat4 MVP = Projection * View * ModelMatrix;

	// Use our shader
	glUseProgram(ShaderID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	if (WireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);
	}
	else
	{
		GLuint LightPositionID = glGetUniformLocation(ShaderID, "LightPosition");
		glUniform3fv(LightPositionID, 16, glm::value_ptr(lightInfo.positions[0]));
		glUniform3fv(LightColorID, 16, glm::value_ptr(lightInfo.colors[0]));
		glUniform1fv(LightPowerID, 16, lightInfo.intensities);
	}

	// Send our transformation to the currently bound shader, in the "MVP" uniform
	// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
	glUniformMatrix4fv(MVPMatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(VMatrixID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

	ObjectMesh->PrepareMesh();
	glDrawArrays(GL_TRIANGLES, 0, ObjectMesh->GetVerticesCount());
	ObjectMesh->Finished();

	if (WireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_CULL_FACE);
	}
}


void GraphicObject::SetTexture(GLuint textureID, const char* path)
{
	TextureID = textureID;
	TexturePath = path;
	//if (TextureID != 0)
	//{
	//	glDeleteTextures(1, &TextureID);
	//}
	//TexturePath = path;
	//TextureID = LoadDDS(path);
}


void GraphicObject::Rotate(glm::vec3 direction, float angle)
{
	rotationAxis = direction;
	Angle = angle;
	Transform(position, rotationAxis, Angle, scale);
	//glm::mat4 transformation = glm::mat4(1.0f);

	//transformation = glm::scale(transformation, scale);
	//transformation = glm::rotate(transformation, glm::radians(Angle), rotationAxis);
	//transformation = glm::translate(transformation, position);
	//ModelMatrix = transformation;
	//
	//ModelMatrix = glm::mat4(1) * glm::scale(scale) * ;
	//ModelMatrix = glm::rotate(ModelMatrix, Angle, rotationAxis);
	//ModelMatrix = glm::translate(ModelMatrix, position/scale);

	//SetPosition(position);
	//ModelMatrix = glm::mat4(1) * glm::scale(scale);
	//ModelMatrix= glm::rotate(ModelMatrix, Angle, rotationAxis);
	//ModelMatrix = ModelMatrix * glm::rotate(glm::radians(angle), direction);
	//Angle = angle;
}

void GraphicObject::SetWireFrame()
{
	WireFrame = true;
}

std::string GraphicObject::ModelMatString()
{
	return glm::to_string(ModelMatrix);
}

void GraphicObject::SetNewShader(GLuint id)
{
	ShaderID = id;

	glUniform1i(glGetUniformLocation(ShaderID, "myTextureSampler"), 0);
	MVPMatrixID = glGetUniformLocation(ShaderID, "MVP");
	VMatrixID = glGetUniformLocation(ShaderID, "V");
	ModelMatrixID = glGetUniformLocation(ShaderID, "M");
	LightColorID = glGetUniformLocation(ShaderID, "LightColor");
	LightPowerID = glGetUniformLocation(ShaderID, "LightPower");
}

void GraphicObject::SetShaderID(GLuint id)
{
	ShaderID = id;
}

GLuint GraphicObject::GetShaderID()
{
	return ShaderID;
}

const char* GraphicObject::GetModelPath()
{
	return ModelPath.c_str();
}

const char* GraphicObject::GetVShader()
{
	return VShaderPath;
}

const char* GraphicObject::GetFShader()
{
	return FShaderPath;
}

glm::mat4 GraphicObject::GetModelMatrix()
{
	return ModelMatrix;
}

void GraphicObject::SetModelMatrix(glm::mat4 matrix)
{
	ModelMatrix = matrix;
}

void GraphicObject::InitialScale(glm::vec3 s)
{
	scale = s;
}

glm::vec3 GraphicObject::GetScale()
{
	return scale;
}

void GraphicObject::SetMeshPointer(Mesh* mesh)
{
	ObjectMesh = mesh;
}

void GraphicObject::Transform(glm::vec3 pos, glm::vec3 rotAxis, float angle, glm::vec3 scaleVec)
{
	position = pos;
	scale = scaleVec;
	rotationAxis = rotAxis;
	Angle = angle;
	glm::mat4 transformation = glm::mat4(1.0f);

	transformation = glm::translate(transformation, position) * glm::rotate(transformation, glm::radians(Angle), rotationAxis) * glm::scale(transformation, scale);
	//transformation = glm::rotate(transformation, glm::radians(Angle), rotationAxis);
	//transformation = glm::translate(transformation, position);
	ModelMatrix = transformation;
}

//void GraphicObject::SetModel(const char* path, Graphics* graphics)
//{
//	//ObjectMesh.~Mesh();
//	ObjectMesh = graphics->CreateMesh(path);
//	//CreateMesh(path);
//	ModelPath = path;
//}

const char* GraphicObject::GetTexture()
{
	return TexturePath.c_str();
}

void GraphicObject::SetLight(bool t)
{
	light = t;
}

bool GraphicObject::IsLight()
{
	return light;
}

//void GraphicObject::SetShader(const char* vPath, const char* fPath)
//{
//	ObjectShader = Shader(vPath, fPath);
//	shaderID = ObjectShader.ID;
//}



//GLuint GraphicObject::LoadDDS(const char* path)
//{
//	unsigned char header[124];
//
//	//FILE* fp;
//	FILE* file;
//	errno_t err;
//
//	err = fopen_s(&file, path, "rb");
//	/* try to open the file */
//	//fp = fopen_s(path, "rb");
//	if (err != 0)
//		return 0;
//
//	/* verify the type of file */
//	char filecode[4];
//	fread(filecode, 1, 4, file);
//	if (strncmp(filecode, "DDS ", 4) != 0) {
//		fclose(file);
//		return 0;
//	}
//
//	/* get the surface desc */
//	fread(&header, 124, 1, file);
//
//	unsigned int height = *(unsigned int*)&(header[8]);
//	unsigned int width = *(unsigned int*)&(header[12]);
//	unsigned int linearSize = *(unsigned int*)&(header[16]);
//	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
//	unsigned int fourCC = *(unsigned int*)&(header[80]);
//
//	unsigned char* buffer;
//	unsigned int bufsize;
//	/* how big is it going to be including all mipmaps? */
//	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
//	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
//	fread(buffer, 1, bufsize, file);
//	/* close the file pointer */
//	fclose(file);
//
//	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
//	unsigned int format;
//	switch (fourCC)
//	{
//	case FOURCC_DXT1:
//		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
//		break;
//	case FOURCC_DXT3:
//		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
//		break;
//	case FOURCC_DXT5:
//		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
//		break;
//	default:
//		free(buffer);
//		return 0;
//	}
//
//	// Create one OpenGL texture
//	//GLuint textureID;
//	glGenTextures(1, &TextureID);
//
//	// "Bind" the newly created texture : all future texture functions will modify this texture
//	glBindTexture(GL_TEXTURE_2D, TextureID);
//
//	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
//	unsigned int offset = 0;
//
//	/* load the mipmaps */
//	if (mipMapCount > 1)
//	{
//		for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
//		{
//			unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
//			glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
//				0, size, buffer + offset);
//
//			offset += size;
//			width /= 2;
//			height /= 2;
//		}
//	}
//	// If dds doesn't have mip maps, generate our own.
//	else
//	{
//		unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
//		glCompressedTexImage2D(GL_TEXTURE_2D, 0, format, width, height,
//			0, size, buffer + offset);
//
//		glGenerateMipmap(GL_TEXTURE_2D);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 3);
//	}
//
//	free(buffer);
//
//	return TextureID;
//}

void GraphicObject::ScaleSetPos(glm::vec3 pos)
{
	ModelMatrix = glm::mat4(1) * glm::scale(scale);
	ModelMatrix = ModelMatrix * glm::translate(pos / scale);
}
