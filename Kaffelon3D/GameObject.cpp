#include "GameObject.h"

GameObject::GameObject(glm::vec3 pos, GraphicObject* graphicObject)
{
	graphics = graphicObject;
	position = pos;
	graphics->SetPosition(position);
	hitboxGraphic = new GraphicObject("shaders/WireVertexShader.vertexshader", "shaders/WireFragmentShader.fragmentshader", "models/cube.txt");
	hitboxGraphic->SetWireFrame();
	RotMatrix = glm::rotate(0.0f, glm::vec3(1, 1, 1));
}

GraphicObject* GameObject::GetGraphics()
{
	return graphics;
}

glm::vec3 GameObject::GetHitBoxMin()
{
	return (hitbox->Min) + position + hitbox->offset;
}

glm::vec3 GameObject::GetHitBoxMax()
{
	return (hitbox->Max) + position + hitbox->offset;
}

HitBox* GameObject::GetHitBox()
{
	return hitbox;
}

void GameObject::SetHitBox(glm::vec3 min, glm::vec3 max)
{
	hitbox = new HitBox(min, max);
}

void GameObject::SetScale(glm::vec3 s)
{
	graphics->SetScale(s);
	if(!overrideHitboxScale)
		ScaleHitbox(s);
}

void GameObject::ScaleHitbox(glm::vec3 s, bool override)
{
	overrideHitboxScale = override;
	glm::vec3 offset = hitbox->offset;
	SetHitBox(glm::vec3(-1) * s, glm::vec3(1) * s);
	//hitboxGraphic->SetScale(s);
	hitbox->scale = s;
	hitbox->offset = offset;

}

void GameObject::SetRotation(glm::vec3 direction, float amount)
{
	RotMatrix = glm::rotate(glm::radians(amount), direction);
	graphics->Rotate(direction, amount);
	//graphics->SetModelMatrix(graphics->GetModelMatrix() * RotMatrix);
	for (int i = 0; i < 8; i++)
	{
		// USE QUATERNIONS LATER maybe
		//hitbox->points[i] = RotMatrix * glm::vec4(hitbox->points[i].x, hitbox->points[i].y, hitbox->points[i].z,1);
		//glRotatef(amount, direction.x, direction.y, direction.z);
	}

	if (hitboxGraphic != nullptr)
		hitboxGraphic->Transform(GetPosition(), direction, amount, hitbox->scale);
		//hitboxGraphic->Rotate(direction, amount);
	hitbox->Rotate(RotMatrix);
}

const glm::vec3 GameObject::GetPosition()
{
	return position;
}

void GameObject::SetName(std::string n)
{
	name = n;
}

std::string GameObject::GetName()
{
	return name;
}

void GameObject::SetPosition(glm::vec3 p)
{
	position = p;
	graphics->SetPosition(p);
	if (ObjectLight != nullptr)
	{
		ObjectLight->SetPosition(p);
	}
}

GraphicObject* GameObject::GetHitboxGraphic()
{
	if (GetHitBoxMin() != GetHitBoxMax())
	{
		hitboxGraphic->SetPosition(position + hitbox->offset);

		hitboxGraphic->SetScale(hitbox->scale);

		return hitboxGraphic;
	}
	else
	{
		return nullptr;
	}

}

bool GameObject::IsCollider()
{
	return collide;
}

void GameObject::SetCollision(bool t)
{
	collide = t;
}

void GameObject::SetObjectLight(Light* l)
{
	ObjectLight = l;
}

Light* GameObject::GetObjectLight()
{
	return ObjectLight;
}

glm::mat4 GameObject::GetRotMatrix()
{

	return RotMatrix;
}

//void GameObject::Duplicate()
//{
//	GameObject* newObject = world->CreateObject(position, modelpath.c_str(), hitboxmin, hitboxmax, vshaderpath.c_str(), fshaderpath.c_str(), modelMatrix, scale);
//	newObject->SetName(name);
//	newObject->GetHitBox()->offset = hitboxoffset;
//	newObject->ScaleHitbox(hitboxscale);
//	newObject->GetGraphics()->SetTexture(texture.c_str());
//	newObject->SetHitboxOverride(hitboxOverrideBool);
//}

bool GameObject::IsLight()
{
	if (ObjectLight == nullptr)
		return false;
	else
		return true;
}
