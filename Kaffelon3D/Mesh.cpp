#include "Mesh.h"


Mesh::Mesh()
{
	VertexArray = 0;
    VertexBuffer = 0;
	UVBuffer = 0;
	NormalBuffer = 0;
	ElementBuffer = 0;
}

void Mesh::SetupBuffers() {

	// This will identify our vertex buffer
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &VertexBuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	//GLuint UVBuffer;
	glGenBuffers(1, &UVBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	//GLuint Normal;
	glGenBuffers(1, &NormalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);
}

void Mesh::PrepareMesh()
{
	glBindVertexArray(VertexArray);
	// set up buffers and attributes.
	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	
	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);

	glVertexAttribPointer(
		1,                  // attribute 
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	
	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	

}

void Mesh::Finished()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindTexture(GL_TEXTURE_2D, 0);
}

int Mesh::GetVerticesCount()
{
	return indexed_vertices.size() * 3;
}

bool Mesh::ImportFromPath(const std::string& pFile)
{
	glGenVertexArrays(1, &VertexArray);
	// Create an instance of the Importer class
	Assimp::Importer importer;

	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// probably to request more postprocessing than we do in this example.
	const aiScene* scene = importer.ReadFile(pFile,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	// If the import failed, report it
	if (nullptr == scene) {
		std::cout << "importer.GetErrorString()" << "\n";
		return false;
	}

	aiMesh* mesh = scene->mMeshes[0];
	std::vector<std::uint32_t> indices;
	indices.reserve(mesh->mNumFaces * 3u);
	for (std::uint32_t faceIndex = 0u; faceIndex < mesh->mNumFaces; faceIndex++) {
		indices.push_back(mesh->mFaces[faceIndex].mIndices[0u]);
		indices.push_back(mesh->mFaces[faceIndex].mIndices[1u]);
		indices.push_back(mesh->mFaces[faceIndex].mIndices[2u]);
	}
	int test = 0;
	for (int i = 0; i < indices.size(); i++) {
		aiVector3D vertex = mesh->mVertices[indices[i]];
		glm::vec3 vertexVec = glm::vec3(vertex.x, vertex.y, vertex.z);
		//mesh->mVertices[i].;
		indexed_vertices.push_back(vertexVec);

		if (mesh->HasTextureCoords(0)) {
			aiVector3D uvCoord = mesh->mTextureCoords[0][indices[i]];
			glm::vec2 uvVec = glm::vec2(uvCoord.x, 1.0-uvCoord.y);
			indexed_uvs.push_back(uvVec);
		}
		if (mesh->HasNormals()) {
			aiVector3D normal = mesh->mNormals[indices[i]];
			glm::vec3 normalVec = glm::vec3(normal.x, normal.y, normal.z);
			indexed_normals.push_back(normalVec);
		}

	}
	SetupBuffers();
}


Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VertexArray);
	glDeleteBuffers(1, &VertexBuffer);
	glDeleteBuffers(1, &UVBuffer);
	glDeleteBuffers(1, &NormalBuffer);
	glDeleteBuffers(1, &ElementBuffer);
}
