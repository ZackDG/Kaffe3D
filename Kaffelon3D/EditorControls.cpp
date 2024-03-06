#include "EditorControls.h"


std::string EditorControls::GetConsoleCommand()
{
	std::string s;
	if (std::cin >> s)
		return s;

	return s;
}

EditorControls::EditorControls(World* w)
{
	ConsoleInput = std::async(std::launch::async, [] {return EditorControls::GetConsoleCommand(); });
	world = w;
}

EditorControls::~EditorControls()
{
	ConsoleInput.~future();
}


void EditorControls::ClickObject(std::vector<GameObject*>* objects, Graphics* graphics, Player* player, Timer* timer)
{
	if (timer->Interval(.2f))
	{
		hoveredObject = Physics::MouseOverObjects(objects, player);
	}
	if (hoveredObject != nullptr)
	{
		if (glfwGetMouseButton(graphics->GetWindow(), GLFW_MOUSE_BUTTON_LEFT) && !clicked)
		{
			clicked = true;
			if (clickedObject != hoveredObject && clickedObject != nullptr)
			{
				clickedObject->GetGraphics()->SetShaderID(prevSID);
			}
			if (clickedObject != hoveredObject)
			{
				clickedObject = hoveredObject;
				prevSID = clickedObject->GetGraphics()->GetShaderID();
				clickedObject->GetGraphics()->SetShaderID(editSID);
			}
		}
		else if (!glfwGetMouseButton(graphics->GetWindow(), GLFW_MOUSE_BUTTON_LEFT) && clicked)
		{
			clicked = false;
		}
	}
	if (glfwGetMouseButton(graphics->GetWindow(), GLFW_MOUSE_BUTTON_3))
	{
		if (clickedObject != nullptr)
		{
			clickedObject->GetGraphics()->SetShaderID(prevSID);
		}
		else
		{
			clickedObject = nullptr;
		}
	}
}

GameObject* EditorControls::GetClickedObject()
{
	return clickedObject;
}

void EditorControls::GameObjectEditControls(GLFWwindow* window)
{
	if (clickedObject != nullptr && !duplicatePressed) {
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) && glfwGetKey(window, GLFW_KEY_D) && glfwGetKey(window, GLFW_KEY_C)) {
			if (clickedObject->IsLight()) {
				GameObject* newObject = world->CreateLight(clickedObject->GetPosition(), clickedObject->GetObjectLight()->GetColor(), clickedObject->GetObjectLight()->GetIntensity());
				std::cout << "duplicated light" ;
			}
			else {
				
				world->DuplicateObject(clickedObject);
			}
			duplicatePressed = true;

		}
	}
	if (!(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) && glfwGetKey(window, GLFW_KEY_D) && glfwGetKey(window, GLFW_KEY_C))) {
		duplicatePressed = false;
	}
}

void EditorControls::MoveClickedObject(GLFWwindow* window, Player* player, float deltaTime)
{
	if (clickedObject != nullptr)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
		{
			glfwGetKey(window, GLFW_KEY_B);
			glfwGetKey(window, GLFW_KEY_5);

			float shift_modifier = 1.0f;
			if (glfwGetKey(window, GLFW_KEY_LEFT_ALT))
				shift_modifier = 10.0F;
			if (glfwGetKey(window, GLFW_KEY_LEFT) && Pressed[4] == false)
			{ 
				Pressed[4] = true;
				clickedObject->SetPosition(clickedObject->GetPosition() + glm::vec3(0, 0, 1 * shift_modifier));
			}
			else if (Pressed[4] && !(glfwGetKey(window, GLFW_KEY_LEFT) && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)))
			{
				Pressed[4] = false;
			}

			if (glfwGetKey(window, GLFW_KEY_RIGHT)&& Pressed[5] == false)
			{
				Pressed[5] = true;
				clickedObject->SetPosition(clickedObject->GetPosition() + glm::vec3(0, 0, -1 * shift_modifier));
			}
			else if (Pressed[5] && !(glfwGetKey(window, GLFW_KEY_RIGHT) && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)))
			{
				Pressed[5] = false;
			}

			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
			{
				glfwGetKey(window, GLFW_KEY_Z);
				glfwGetKey(window, GLFW_KEY_X);
				glfwGetKey(window, GLFW_KEY_C);

				if (glfwGetKey(window, GLFW_KEY_Z))
				{
					float sign = 1;
					if (player->GetPosition().z > clickedObject->GetPosition().z)
					{
						sign = -1;
					}
					clickedObject->GetHitBox()->SetOffset(clickedObject->GetHitBox()->offset + glm::vec3(player->GetControls()->GetDeltaHAngle() * 1.0f * sign * deltaTime * moveSpeed, 0, 0));
				}
				if (glfwGetKey(window, GLFW_KEY_X))
				{
					clickedObject->GetHitBox()->SetOffset(clickedObject->GetHitBox()->offset + glm::vec3(0, player->GetControls()->GetDeltaVAngle() * 1.0f * deltaTime * moveSpeed, 0));
				}
				if (glfwGetKey(window, GLFW_KEY_C))
				{
					float sign = 1;
					if (player->GetPosition().x < clickedObject->GetPosition().x)
					{
						sign = -1;
					}
					clickedObject->GetHitBox()->SetOffset(clickedObject->GetHitBox()->offset + glm::vec3(0, 0, player->GetControls()->GetDeltaHAngle() * 1.0f * sign * deltaTime * moveSpeed));
				}



			}
			if (glfwGetKey(window, GLFW_KEY_B))
			{
				clickedObject->GetHitBox()->SetOffset(glm::vec3(0, 0, 0));
			}
			if (glfwGetKey(window, GLFW_KEY_5))
			{
				clickedObject->ScaleHitbox(clickedObject->GetGraphics()->GetScale());
			}
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
		{
			glfwGetKey(window, GLFW_KEY_Z);
			glfwGetKey(window, GLFW_KEY_X);
			glfwGetKey(window, GLFW_KEY_C);

			if (glfwGetKey(window, GLFW_KEY_Z))
			{
				float sign = 1;
				if (player->GetPosition().z > clickedObject->GetPosition().z)
				{
					sign = -1;
				}
				clickedObject->SetPosition(clickedObject->GetPosition() + glm::vec3(player->GetControls()->GetDeltaHAngle() * 1.0f * sign * deltaTime * moveSpeed, 0, 0));
			}
			if (glfwGetKey(window, GLFW_KEY_X))
			{
				clickedObject->SetPosition(clickedObject->GetPosition() + glm::vec3(0, player->GetControls()->GetDeltaVAngle() * 1.0f * deltaTime * moveSpeed, 0));
			}
			if (glfwGetKey(window, GLFW_KEY_C))
			{
				float sign = 1;
				if (player->GetPosition().x < clickedObject->GetPosition().x)
				{
					sign = -1;
				}
				clickedObject->SetPosition(clickedObject->GetPosition() + glm::vec3(0, 0, player->GetControls()->GetDeltaHAngle() * 1.0f * sign * deltaTime * moveSpeed));
			}
		}
		else
		{
			float shift_modifier = 1.0f;
			if (glfwGetKey(window, GLFW_KEY_LEFT_ALT))
				shift_modifier = 10.0F;
			if (glfwGetKey(window, GLFW_KEY_LEFT) && Pressed[0] == false)
			{
				Pressed[0] = true;
				clickedObject->SetPosition(clickedObject->GetPosition() + (glm::vec3(-1, 0, 0)) * shift_modifier);
			}
			else if (Pressed[0] && !glfwGetKey(window, GLFW_KEY_LEFT))
			{
				Pressed[0] = false;
			}

			if (glfwGetKey(window, GLFW_KEY_RIGHT) && Pressed[1] == false)
			{
				Pressed[1] = true;
				clickedObject->SetPosition(clickedObject->GetPosition() + (glm::vec3(1, 0, 0)) * shift_modifier);
			}
			else if (Pressed[1] && !glfwGetKey(window, GLFW_KEY_RIGHT))
			{
				Pressed[1] = false;
			}

			if (glfwGetKey(window, GLFW_KEY_DOWN) && Pressed[2] == false)
			{
				Pressed[2] = true;
				clickedObject->SetPosition(clickedObject->GetPosition() + (glm::vec3(0, -1, 0)) * shift_modifier);
			}
			else if (Pressed[2] && !glfwGetKey(window, GLFW_KEY_DOWN))
			{
				Pressed[2] = false;
			}

			if (glfwGetKey(window, GLFW_KEY_UP) && Pressed[3] == false)
			{
				Pressed[3] = true;
				clickedObject->SetPosition(clickedObject->GetPosition() + (glm::vec3(0, 1, 0)) * shift_modifier);
			}
			else if (Pressed[3] && !glfwGetKey(window, GLFW_KEY_UP))
			{
				Pressed[3] = false;
			}
		}	
	}
}

void EditorControls::ScaleClickedObject(GLFWwindow* window, Player* player, float deltaTime)
{
	if (clickedObject != nullptr)
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
		{
			glfwGetKey(window, GLFW_KEY_1);
			glfwGetKey(window, GLFW_KEY_2);
			glfwGetKey(window, GLFW_KEY_3);

			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
			{
				if (glfwGetKey(window, GLFW_KEY_1))
				{
					float sign = 1;
					if (player->GetPosition().z > clickedObject->GetPosition().z)
					{
						sign = -1;
					}
					clickedObject->ScaleHitbox(clickedObject->GetHitBox()->scale + glm::vec3(player->GetControls()->GetDeltaHAngle() * 1.0f * sign * deltaTime * moveSpeed, 0, 0), true);
				}
				if (glfwGetKey(window, GLFW_KEY_2))
				{
					clickedObject->ScaleHitbox(clickedObject->GetHitBox()->scale + glm::vec3(0, player->GetControls()->GetDeltaVAngle() * 1.0f * deltaTime * moveSpeed, 0), true);
				}
				if (glfwGetKey(window, GLFW_KEY_3))
				{
					float sign = 1;
					if (player->GetPosition().x < clickedObject->GetPosition().x)
					{
						sign = -1;
					}
					clickedObject->ScaleHitbox(clickedObject->GetHitBox()->scale + glm::vec3(0, 0, player->GetControls()->GetDeltaHAngle() * 1.0f * sign * deltaTime * moveSpeed), true);
				}
			}
			else
			{
				if (glfwGetKey(window, GLFW_KEY_1))
				{
					float sign = 1;
					if (player->GetPosition().z > clickedObject->GetPosition().z)
					{
						sign = -1;
					}
					//clickedObject->SetPosition(clickedObject->GetPosition() + glm::vec3(player->GetControls()->GetDeltaHAngle() * 1.0f * sign, 0, 0));
					clickedObject->SetScale(clickedObject->GetGraphics()->GetScale() + glm::vec3(player->GetControls()->GetDeltaHAngle() * 1.0f * sign * deltaTime * scaleSpeed, 0, 0));
					//clickedObject->SetScale(glm::vec3(2));
				}
				if (glfwGetKey(window, GLFW_KEY_2))
				{
					//clickedObject->SetPosition(clickedObject->GetPosition() + glm::vec3(0, player->GetControls()->GetDeltaVAngle() * 100.0f, 0));
					clickedObject->SetScale(clickedObject->GetGraphics()->GetScale() + glm::vec3(0, player->GetControls()->GetDeltaVAngle() * 1.0f * deltaTime * scaleSpeed, 0));

				}
				if (glfwGetKey(window, GLFW_KEY_3))
				{
					float sign = 1;
					if (player->GetPosition().x < clickedObject->GetPosition().x)
					{
						sign = -1;
					}
					clickedObject->SetScale(clickedObject->GetGraphics()->GetScale() + glm::vec3(0, 0, player->GetControls()->GetDeltaHAngle() * 1.0f * deltaTime * scaleSpeed));
					//clickedObject->SetPosition(clickedObject->GetPosition() + glm::vec3(0, 0, player->GetControls()->GetDeltaHAngle() * 100.0f * sign));
				}
			}
		}
	}
}


void EditorControls::CheckConsoleInput()
{
	if (ConsoleInput._Is_ready()) {
		std::string input = ConsoleInput.get();
		// Commands where a object must be selected
		if (clickedObject != nullptr)
		{
			if (input == "Delete")
			{
				std::string name = clickedObject->GetName();
				world->DeleteObject(clickedObject);
				std::cout << "Deleted: " << name << "\n";
				clickedObject = nullptr;
			}
			else if (input == "Position")
			{
				//clickedObject->SetPosition()
				command = input;
			}
			else if (command == "Position")
			{
				//if (clickedObject == nullptr)
				//{
				//	std::cout << "No Object Selected!" << "\n";
				//}
				glm::vec3 position = glm::vec3(0);

				try {
					std::string substr = input.substr(0, input.find(','));
					position.x = std::stof(substr, nullptr);
					substr = input.substr(input.find(',') + 1, input.npos);
					position.y = std::stof(substr, nullptr);
					substr = substr.substr(substr.find(',') + 1, substr.npos);
					position.z = std::stof(substr, nullptr);

					clickedObject->SetPosition(position);
					std::cout << "Set " << clickedObject->GetName() << "'s" << " position to: " << glm::to_string(position) << "\n";
				}
				catch (std::invalid_argument)
				{
					std::cout << "Bad Position Command! " << "\n";
				}
				catch (std::out_of_range)
				{
					std::cout << "Bad Position Command! " << "\n";
				}

				command = "";

				//input = input.substr(0, 1);
			}
			else if (input == "Rotate") {
				command = input;
			}
			else if (command == "Rotate") {
				glm::vec3 rotateDir = glm::vec3(0);
				float rotateAmount = 0;
				std::string substr = input.substr(0, input.find(','));
				rotateDir.x = std::stof(substr);
				substr = input.substr(input.find(',') + 1, input.npos);
				rotateDir.y = std::stof(substr);
				substr = substr.substr(substr.find(',') + 1, substr.npos);
				rotateDir.z = std::stof(substr);
				substr = substr.substr(substr.find(',') + 1, substr.npos);
				rotateAmount = std::stof(substr);
				clickedObject->SetRotation(rotateDir, rotateAmount);
				std::cout << "Set " << clickedObject->GetName() << "'s" << " rotation to: " << glm::to_string(rotateDir*rotateAmount) << "\n";
			}
			else if (input == "Scale") {
				command = input;
			}
			else if (command == "Scale") {
				glm::vec3 scale = glm::vec3(1);
				std::string substr = input.substr(0, input.find(','));
				scale.x = std::stof(substr, nullptr);
				substr = input.substr(input.find(',') + 1, input.npos);
				scale.y = std::stof(substr, nullptr);
				substr = substr.substr(substr.find(',') + 1, substr.npos);
				scale.z = std::stof(substr, nullptr);
				clickedObject->SetScale(scale);
				std::cout << "Set " << clickedObject->GetName() << "'s" << " scale to: " << glm::to_string(scale) << "\n";
			}
			//else if (input == "NoCollide"){ 
			//	command = input;
			//}
			else if (input == "NoCollide") {
				clickedObject->SetNoCollide();
				std::cout << "Set " << clickedObject->GetName() << " to no collide" << "\n";
			}
			else if (input == "Model")
			{
				command = input;
			}
			else if (command == "Model")
			{
				std::string modelPath = "models/" + input;
				world->GetGraphics()->SetModel(clickedObject->GetGraphics(), modelPath.c_str());
				std::cout << clickedObject->GetName() << " model changed to: " << modelPath << "\n";
				command = "";
			}
			else if (input == "Name")
			{
				command = input;

			}
			else if (command == "Name")
			{
				clickedObject->SetName(input);
				std::cout << "Object name set to: " << clickedObject->GetName() << "\n";
				command = "";
			}
			else if (input == "Texture")
			{
				command = input;
			}
			else if (command == "Texture")
			{
				std::string texturePath = "textures/" + input;
				GLuint textureID = world->GetGraphics()->CreateTexture(texturePath);
				clickedObject->GetGraphics()->SetTexture(textureID, texturePath.c_str());
				std::cout << clickedObject->GetName() << " texture set to: " << texturePath << "\n";
				command = "";
			}
			else if (input == "Color")
			{
				command = input;
			}
			else if (command == "Color" && clickedObject->GetObjectLight() != nullptr)
			{
				glm::vec3 color = glm::vec3(0);
				try {
					std::string substr = input.substr(0, input.find(','));
					color.x = std::stof(substr, nullptr);
					substr = input.substr(input.find(',') + 1, input.npos);
					color.y = std::stof(substr, nullptr);
					substr = substr.substr(substr.find(',') + 1, substr.npos);
					color.z = std::stof(substr, nullptr);

					clickedObject->GetObjectLight()->SetColor(color);
					std::cout << "Set " << clickedObject->GetName() << "'s" << " color to: " << glm::to_string(color) << "\n";
				}
				catch (std::invalid_argument)
				{
					std::cout << "Incorrectly Formatted Vector! " << "\n";
				}
				catch (std::out_of_range)
				{
					std::cout << "Incorrectly Formatted Vector! " << "\n";
				}

				command = "";
			}
			else if (command == "Color" && clickedObject->GetObjectLight() == nullptr)
			{
			std::cout << "Tried to set Color For non-light" << "\n";
			}
			else if (input == "Power") {
				command = input;
				
			}
			else if (command == "Power") {
				clickedObject->GetObjectLight()->SetIntensity(std::stof(input, nullptr));
			}
		}

		if (input == "CreateObject")
		{
			GameObject* newObject = world->CreateObject(glm::vec3(0, 0, 0), "models/cube.txt");
			newObject->SetHitBox(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1));
			command = input;
			std::cout << "Created New Object at " << glm::to_string(glm::vec3(0, 0, 0)) << "\n";
		}

		else if (input == "CreateLight")
		{
			GameObject* newObject = world->CreateLight(glm::vec3(0, 0, 0), glm::vec3(.7, 0, .7), 5.0f);
			std::cout << "Created New Light" << "\n";

		}

		else if (clickedObject == nullptr)
		{
			std::cout << "No Object Selected" << "\n";
		}
		ConsoleInput = std::async(std::launch::async, [] {return EditorControls::GetConsoleCommand(); });
	}
}
