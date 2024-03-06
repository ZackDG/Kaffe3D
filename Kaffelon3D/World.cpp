#include "World.h"

World::World(bool isEdit)
{
	edit = isEdit;
	graphics = new Graphics(isEdit);
	GameObjects = new std::vector<GameObject*>;
}


GameObject* World::CreateObject(glm::vec3 pos, const char* modelPath)
{
	GameObject* newGameObject = CreateObject(pos, modelPath, glm::vec3(-1), glm::vec3(1));
	return newGameObject;
}
GameObject* World::CreateObject(glm::vec3 pos, const char* modelPath, glm::vec3 hitboxmin, glm::vec3 hitboxmax)
{
	GameObject* newGameObject = CreateObject(pos, modelPath, hitboxmin, hitboxmax, "shaders/SimpleVertexShader.vertexshader", "shaders/SimpleFragmentShader.fragmentshader", glm::mat4(1), glm::vec3(1, 1, 1));
	return newGameObject;
}

GameObject* World::CreateObject(glm::vec3 pos, const char* modelPath, glm::vec3 hitboxmin, glm::vec3 hitboxmax, const char* vshader, const char* fshader, glm::mat4 modelMatrix, glm::vec3 scale)
{
	GraphicObject* newGraphicObject = graphics->CreateGraphicObject(vshader, fshader, modelPath);
	newGraphicObject->SetModelMatrix(modelMatrix);
	glm::vec3 position = glm::vec3(modelMatrix[3].x, modelMatrix[3].y, modelMatrix[3].z);
	newGraphicObject->SetPosition(position);
	newGraphicObject->InitialScale(scale);

	GameObject* newGameObject = new GameObject(pos, newGraphicObject);
	newGameObject->SetHitBox(hitboxmin, hitboxmax);
	GameObjects->push_back(newGameObject);
	HitboxInit(newGameObject);
	return newGameObject;
}

void World::DuplicateObject(GameObject* o)
{
	std::string modelPath = o->GetGraphics()->GetModelPath();
	glm::vec3 hitboxMin = o->GetHitBoxMin();
	glm::vec3 hitboxMax = o->GetHitBoxMax();
	std::string vertexShaderPath = o->GetGraphics()->GetVShader();
	std::string fragmentShaderPath = o->GetGraphics()->GetFShader();
	glm::mat4 modelMatrix = o->GetGraphics()->GetModelMatrix();
	glm::vec3 scale = o->GetGraphics()->GetScale();
	GameObject* newObject = CreateObject(o->GetPosition(), modelPath.c_str(), hitboxMin, hitboxMax, vertexShaderPath.c_str(), fragmentShaderPath.c_str(), modelMatrix, scale);
	newObject->SetRotation(o->GetGraphics()->GetRotationAxis(), o->GetGraphics()->GetAngle());
	newObject->SetName(o->GetName());
	newObject->GetHitBox()->offset = o->GetHitBox()->offset;
	newObject->ScaleHitbox(o->GetHitBox()->scale);
	GLuint textureID = GetGraphics()->CreateTexture(o->GetGraphics()->GetTexture());
	newObject->GetGraphics()->SetTexture(textureID, o->GetGraphics()->GetTexture());
	newObject->SetHitboxOverride(o->IsHitboxOverridden());
}

void World::DeleteObject(GameObject* o)
{
	std::vector<GameObject*>::iterator it = std::find(GameObjects->begin(), GameObjects->end(), o);	
	graphics->DeleteGraphicObject((*it)->GetGraphics());
	GameObjects->erase(it);
}



Text World::CreateText(glm::vec3 color, glm::vec2 pos, std::string text, int size)
{
	Text* newText = new Text(color, pos, text, size, graphics->GetScreenSize().x, graphics->GetScreenSize().y);
	TextObjects.push_back(newText);
	return *newText;
}

GLFWwindow* World::GetWindow()
{
	return graphics->GetWindow();
}

std::vector<GameObject*>* World::GetGameObjects()
{
	return GameObjects;
}

Graphics* World::GetGraphics()
{
	return graphics;
}



void World::RenderHitBoxes(glm::mat4 Projection, glm::mat4 View)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	for(GameObject* it : *GameObjects)
	{
		if (it->GetHitboxGraphic() != nullptr)
		{
			graphics->RenderGraphic(it->GetHitboxGraphic(), Projection, View);
		}
	}
}

void World::HitboxInit(GameObject* object)
{
	object->GetHitboxGraphic()->SetMeshPointer(graphics->CreateMesh("models/cube.txt"));
	Shader* newShader = graphics->CreateShader("shaders/WireVertexShader.vertexshader", "shaders/WireFragmentShader.fragmentshader");
	object->GetHitboxGraphic()->SetNewShader(newShader->ID);
}

void World::SetPosition(glm::vec3* objectPos, glm::vec3 newPos)
{
	*objectPos = newPos;
}

GameObject* World::CreateLight(glm::vec3 pos, glm::vec3 col, float intens)
{
	GameObject* newObject = CreateObject(pos, "models/cube.txt", glm::vec3(-.25, -.25, -.25), glm::vec3(.25, .25, .25), "shaders/WireVertexShader.vertexshader", "shaders/WireFragmentShader.fragmentshader", glm::mat4(1), glm::vec3(.5f));
	Light* newLight = graphics->CreateLight(newObject, col, intens);
	newObject->ScaleHitbox(glm::vec3(.5f));
	newObject->SetObjectLight(newLight);
	newObject->SetCollision(false);
	newObject->GetGraphics()->SetWireFrame();
	return newObject;
}
