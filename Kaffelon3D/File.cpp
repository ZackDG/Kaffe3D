#include "File.h"

struct File::save_data {
	std::string name;
	glm::vec3 position;
	glm::vec3 rotationaxis;
	float angle;
	glm::vec3 hitboxmin;
	glm::vec3 hitboxmax;
	glm::vec3 hitboxoffset;
	glm::vec3 hitboxscale;
	glm::vec3 scale;
	const char* model;
	const char* vshader;
	const char* fshader;
	const char* texture;
	glm::mat4 modelmat;
	bool isHitboxOverriden;
	bool isCollider;
	bool isLight;
	glm::vec3 lightcolor;
	float lightIntensity;
};

void File::SaveObjects(std::vector<GameObject*>* objects, const char* outputPath)
{
	std::ofstream outfile(outputPath, std::ofstream::out | std::ofstream::trunc);

	for (GameObject* it : *objects)
	{
		SaveObject(it, &outfile);
	}
	outfile.close();
	std::cout << "Map Saved!\n";
	return;
}

void File::LoadObjects(const char* filepath, World* world)
{
	glm::vec3 position = glm::vec3(0);
	glm::vec3 rotationAxis = glm::vec3(0,1,0);
	float angle = 0;
	glm::vec3 hitboxmin = glm::vec3(99.0f);
	glm::vec3 hitboxmax = glm::vec3(99.0f);
	glm::vec3 hitboxoffset = glm::vec3(99.0f);
	glm::vec3 hitboxscale = glm::vec3(99.0f);
	glm::vec3 lightcolor = glm::vec3(99.0f);
	float lightIntensity = 10.0f;

	glm::vec3 scale = glm::vec3(99.0f);
	glm::mat4 modelMatrix = glm::mat4(99.0f);
	std::string modelpath;
	std::string vshaderpath;
	std::string fshaderpath;
	std::string texture;
	std::string name;
	bool hitboxOverrideBool = false;
	bool isCollider = true;
	bool isLight = false;

	std::string* xSub = new std::string();
	std::string* ySub = new std::string();
	std::string* zSub = new std::string();

	std::ifstream infile(filepath);
	std::string line;
	while (std::getline(infile, line))
	{
		std::string firstWord = line.substr(0, line.find(':'));
		if (firstWord == "position")
		{
			GetVector3FromString(line, xSub, ySub, zSub);

			position.x = std::stof(*xSub);
			position.y = std::stof(*ySub);
			position.z = std::stof(*zSub);
		}
		else if (firstWord == "rotationaxis") {
			GetVector3FromString(line, xSub, ySub, zSub);

			rotationAxis.x = std::stof(*xSub);
			rotationAxis.y = std::stof(*ySub);
			rotationAxis.z = std::stof(*zSub);
		}
		else if (firstWord == "angle") {
			std::string subString = line.substr(line.find(":") + 1, line.npos);
			angle = std::stof(subString);
		}
		else if (firstWord == "hitboxmin")
		{
			GetVector3FromString(line, xSub, ySub, zSub);

			hitboxmin.x = std::stof(*xSub);
			hitboxmin.y = std::stof(*ySub);
			hitboxmin.z = std::stof(*zSub);
		}
		else if (firstWord == "hitboxmax")
		{
			GetVector3FromString(line, xSub, ySub, zSub);

			hitboxmax.x = std::stof(*xSub);
			hitboxmax.y = std::stof(*ySub);
			hitboxmax.z = std::stof(*zSub);
		}
		else if (firstWord == "hitboxoffset")
		{
			GetVector3FromString(line, xSub, ySub, zSub);

			hitboxoffset.x = std::stof(*xSub);
			hitboxoffset.y = std::stof(*ySub);
			hitboxoffset.z = std::stof(*zSub);
		}
		else if (firstWord == "hitboxscale")
		{
			GetVector3FromString(line, xSub, ySub, zSub);

			hitboxscale.x = std::stof(*xSub);
			hitboxscale.y = std::stof(*ySub);
			hitboxscale.z = std::stof(*zSub);
		}
		else if (firstWord == "scale")
		{
			GetVector3FromString(line, xSub, ySub, zSub);

			scale.x = std::stof(*xSub);
			scale.y = std::stof(*ySub);
			scale.z = std::stof(*zSub);
		}
		else if (firstWord == "model")
		{
			modelpath = line.substr(line.find(":") + 1, line.npos);
		}
		else if (firstWord == "vshader")
		{
			vshaderpath = line.substr(line.find(":") + 1, line.npos);
		}
		else if (firstWord == "fshader")
		{
			fshaderpath = line.substr(line.find(":") + 1, line.npos);
		}
		else if (firstWord == "texture")
		{
			texture = line.substr(line.find(":") + 1, line.npos);
		}
		else if (firstWord == "modelmat")
		{
			modelMatrix = GetMatrix4FromString(line);
		}
		else if (firstWord == "name")
		{
			name = line.substr(line.find(":") + 1, line.npos);
		}
		else if (firstWord == "light")
		{
			if (line.substr(line.find(":") + 1, line.npos) == "1")
				isLight = true;			
		}
		else if (firstWord == "lightcolor")
		{
			GetVector3FromString(line, xSub, ySub, zSub);
			lightcolor.x = std::stof(*xSub);
			lightcolor.y = std::stof(*ySub);
			lightcolor.z = std::stof(*zSub);
		}
		else if (firstWord == "hitboxoverride") {

			if (line.substr(line.find(":") + 1, line.npos) == "1")
				hitboxOverrideBool = true;

		}
		else if (firstWord == "iscollider") {
			if (line.substr(line.find(":") + 1, line.npos) == "0")
				isCollider = false;
		}
		else if (firstWord == "power") {
			std::string substr = line.substr(line.find(":") + 1, line.npos);;
			lightIntensity = std::stof(substr);
		}
		else if (firstWord == "END")
		{
			if (isLight)
			{
				GameObject* newLight = world->CreateLight(position, lightcolor, 10.0f);
				newLight->GetObjectLight()->SetIntensity(lightIntensity);
			}
			else
			{
				if (name == "altar") {
					int debug = -1;
				}
				GameObject* newObject = world->CreateObject(position, modelpath.c_str(), hitboxmin, hitboxmax, vshaderpath.c_str(), fshaderpath.c_str(), modelMatrix, scale);
				newObject->SetName(name);
				newObject->GetHitBox()->offset = hitboxoffset;
				newObject->ScaleHitbox(hitboxscale);
				GLuint textureID = world->GetGraphics()->CreateTexture(texture.c_str());
				newObject->GetGraphics()->SetTexture(textureID, texture.c_str());
				//world->GetGraphics()->SetModel(newObject->GetGraphics(), modelpath.c_str());
				newObject->SetHitboxOverride(hitboxOverrideBool);
				if(angle != 0)
					newObject->SetRotation(rotationAxis, angle);
				if (!isCollider)
					newObject->SetNoCollide();
				name = "";
			}
			rotationAxis = glm::vec3(0,1,0);
			angle = 0;
			hitboxmin = glm::vec3(99.0f);
			hitboxmax = glm::vec3(99.0f);
			hitboxoffset = glm::vec3(99.0f);
			hitboxscale = glm::vec3(99.0f);
			position = glm::vec3(99.0f);
			scale = glm::vec3(99.0f);
			lightcolor = glm::vec3(99.0f);
			lightIntensity = 10.0f;
			modelpath = "GARBAGE";
			vshaderpath = "GARBAGE";
			fshaderpath = "GARBAGE";
			texture = "GARBAGE";
			hitboxOverrideBool = false;
			isCollider = true;
			isLight = false;
			*xSub = std::string("");
			*ySub = std::string("");
			*zSub = std::string("");
		}
	}
	delete xSub;
	delete ySub;
	delete zSub;
	return;
}

void File::SaveObject(GameObject* o, std::ofstream* outfile)
{
	glm::vec3 lightcolor = glm::vec3(0);
	float lightPower = 0.0f;
	if (o->GetObjectLight() != nullptr)
	{
		lightcolor = o->GetObjectLight()->GetColor();
		lightPower = o->GetObjectLight()->GetIntensity();
	}
	save_data save = {
	o->GetName(),
	o->GetPosition(),
	o->GetGraphics()->GetRotationAxis(),
	o->GetGraphics()->GetAngle(),
	o->GetHitBoxMin() - o->GetPosition(),
	o->GetHitBoxMax() - o->GetPosition(),
	o->GetHitBox()->offset,
	o->GetHitBox()->scale,
	o->GetGraphics()->GetScale(),
	o->GetGraphics()->GetModelPath(),
	o->GetGraphics()->GetVShader(),
	o->GetGraphics()->GetFShader(),
	o->GetGraphics()->GetTexture(),
	o->GetGraphics()->GetModelMatrix(),
	o->IsHitboxOverridden(),
	o->IsCollider(),
	o->IsLight(),
	lightcolor,
	lightPower
	};

	*outfile
		<< "name:" << save.name << "\n"
		<< "position:" << glm::to_string(save.position) << "\n"
		<< "rotationaxis:" << glm::to_string(save.rotationaxis) << "\n"
		<< "angle:" << save.angle << "\n"
		<< "hitboxmin:" << glm::to_string(save.hitboxmin) << "\n"
		<< "hitboxmax:" << glm::to_string(save.hitboxmax) << "\n"
		<< "hitboxoffset:" << glm::to_string(save.hitboxoffset) << "\n"
		<< "hitboxscale:" << glm::to_string(save.hitboxscale) << "\n"
		<< "scale:" << glm::to_string(save.scale) << "\n"
		<< "model:" << save.model << "\n"
		<< "vshader:" << save.vshader << "\n"
		<< "fshader:" << save.fshader << "\n"
		<< "texture:" << save.texture << "\n"
		<< "modelmat:" << glm::to_string(save.modelmat) << "\n"
		<< "hitboxoverride:" << save.isHitboxOverriden << "\n"
		<< "iscollider:" << save.isCollider << "\n"
		<< "light:" << save.isLight << "\n"
		<< "lightcolor:" << glm::to_string(save.lightcolor) << "\n"
		<< "power:" << (save.lightIntensity) << "\n"
		<< "END" <<
		std::endl;
	return;
}

void File::GetVector3FromString(std::string line, std::string* xSub, std::string* ySub, std::string* zSub)
{
	*xSub = line.substr(line.find('(') + 1, line.find(',') - line.find('('));
	line = line.substr(line.find(',') + 1);
	*ySub = line.substr(1, line.find(','));
	line = line.substr(line.find(',') + 1);
	*zSub = line.substr(1, line.find(')') - 1);
}

std::string File::GetVector4FromString(std::string line, std::string* x, std::string* y, std::string* z, std::string* w)
{
	*x = line.substr(line.find('(') + 1, line.find(',') - line.find('('));
	line = line.substr(line.find(',') + 1);
	*y = line.substr(1, line.find(','));
	line = line.substr(line.find(',') + 1);
	*z = line.substr(1, line.find(','));
	line = line.substr(line.find(',') + 1);
	*w = line.substr(1, line.find(')') - 1);
	return line.substr(line.find(')'), line.npos);
}

glm::mat4 File::GetMatrix4FromString(std::string line)
{
	std::string* x0 = new std::string();
	std::string* x1 = new std::string();
	std::string* x2 = new std::string();
	std::string* x3 = new std::string();
	std::string* y0 = new std::string();
	std::string* y1 = new std::string();
	std::string* y2 = new std::string();
	std::string* y3 = new std::string();
	std::string* z0 = new std::string();
	std::string* z1 = new std::string();
	std::string* z2 = new std::string();
	std::string* z3 = new std::string();
	std::string* w0 = new std::string();
	std::string* w1 = new std::string();
	std::string* w2 = new std::string();
	std::string* w3 = new std::string();

	glm::mat4 newMatrix = glm::mat4();
	glm::vec4 vec0 = glm::vec4(99.0f);
	glm::vec4 vec1 = glm::vec4(99.0f);
	glm::vec4 vec2 = glm::vec4(99.0f);
	glm::vec4 vec3 = glm::vec4(99.0f);

	//1st vector
	line = GetVector4FromString(line.substr(line.find("(") + 1, line.npos), x0, y0, z0, w0);
	vec0 = glm::vec4(std::stof(*x0), std::stof(*y0), std::stof(*z0), std::stof(*w0));

	line = GetVector4FromString(line.substr(line.find("(") + 1, line.npos), x1, y1, z1, w1);
	vec1 = glm::vec4(std::stof(*x1), std::stof(*y1), std::stof(*z1), std::stof(*w1));

	line = GetVector4FromString(line.substr(line.find("(") + 1, line.npos), x2, y2, z2, w2);
	vec2 = glm::vec4(std::stof(*x2), std::stof(*y2), std::stof(*z2), std::stof(*w2));

	GetVector4FromString(line.substr(line.find("(") + 1, line.npos), x3, y3, z3, w3);
	vec3 = glm::vec4(std::stof(*x3), std::stof(*y3), std::stof(*z3), std::stof(*w3));

	newMatrix = glm::mat4(vec0, vec1, vec2, vec3);

	delete x0;
	delete x1;
	delete x2;
	delete x3;
	delete y0;
	delete y1;
	delete y2;
	delete y3;
	delete z0;
	delete z1;
	delete z2;
	delete z3;
	delete w0;
	delete w1;
	delete w2;
	delete w3;
	return newMatrix;
}

