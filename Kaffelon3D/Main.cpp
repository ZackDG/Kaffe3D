#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <iostream>
#include <vector>
#include <windows.h>
#include <map>
#include <string>
#include <glm/gtx/string_cast.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "AudioFile.h"
#include "TestShader.H"
#include "EditorControls.h"
#include "QuadGraphic.h"
#include "TestShader.h"
#include "GraphicObject.h"
#include "Text.h"
#include "World.h"
#include "Player.h"
#include "HitBox.h"
#include "Physics.h"
#include "Button.h"
#include "Controls.h"
#include "Timer.h";

#include "File.h"
#include "Audio.h"


#define STB_IMAGE_IMPLEMENTATION

enum Side { LEFT, RIGHT };

// Move into its own class later.
void PlayStepAudio(SoundSource* pSound, Timer* pTimer, glm::vec3 velocity, bool isOnGround, Side* footSide) {
	
	if (abs(velocity.x) + abs(velocity.z) > .1 && pTimer->Interval(.4f) && isOnGround)
	{
		if (*footSide == LEFT) {
			pSound->SetPosition(glm::vec3(-2.0f, -5.0f, -1.5f));
			*footSide = RIGHT;
		}
		else {
			pSound->SetPosition(glm::vec3(-2.0f, -5.0f, 1.5f));
			*footSide = LEFT;
		}

		pSound->Play();
	}
	else
	{
		pTimer->UpdateInterval(.5f);
	}
}

int main(void)
{
	const bool IS_EDIT_MODE = false;
	const bool IS_PLAY_MODE = !IS_EDIT_MODE;
	
	World* world = new World(IS_EDIT_MODE);
	const float SCREEN_WIDTH = world->GetGraphics()->SCREEN_WIDTH;
	const float SCREEN_HEIGHT = world->GetGraphics()->SCREEN_HEIGHT;

	GLFWwindow* pWindow = world->GetGraphics()->GetWindow();

	// Load the map and save a backup
	File::LoadObjects("maps/showcase1.txt", world);
	world->GetGraphics()->PostMapLoadUpdate();
	File::SaveObjects(world->GetGameObjects(), "maps/backup.txt");
	// Initialize timer, player, and physics
	Timer* timer = new Timer();
	Player* player = new Player(world->GetGraphics()->GetWindow());
	Physics physics = Physics(timer);

	//Setup footstep audio
	Side footSide = LEFT;
	Audio audio = Audio();
	SoundSource* footstepSound = audio.CreateSoundSource("footstep.wav", glm::vec3(0, -.5f, 0));
	
	//Setup Editor controls
	EditorControls* editControls = NULL;
	if(IS_EDIT_MODE)
		editControls = new EditorControls(world);

	// Setup UI text, buttons, and quads.
	std::vector<Text*> textObjects;
	std::vector<Button*> buttonObjects;
	std::vector<QuadGraphic*> quadObjects;

	Text* text = new Text(glm::vec3(0, 1, 0), glm::vec2(2, 96), "Kaffe3D", 48, SCREEN_WIDTH, SCREEN_HEIGHT);
	Text* FPSText = new Text(glm::vec3(0, 1, 0), glm::vec2(2, 93), "FRAME COUNTER", 48, SCREEN_WIDTH, SCREEN_HEIGHT);
	Text* Selected = new Text(glm::vec3(0, 1, 0), glm::vec2(25, SCREEN_HEIGHT - 120.0f), "Object Hovered", 48, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	textObjects.push_back(text);
	textObjects.push_back(FPSText);
	textObjects.push_back(Selected);

	// HP Bar
	QuadGraphic* healthBarBackground = new QuadGraphic("", glm::vec2(67.25, 94), 31.5, 5, glm::vec4(0.0, 0.0, 0.0, 1), SCREEN_WIDTH, SCREEN_HEIGHT, world->GetGraphics()->GetWindow(), NULL);
	QuadGraphic* healthBar = new QuadGraphic("", glm::vec2(68, 95), 30, 3, glm::vec4(1.0, .3, .3, 1), SCREEN_WIDTH, SCREEN_HEIGHT, world->GetGraphics()->GetWindow(), NULL);
	
	quadObjects.push_back(healthBar);
	quadObjects.push_back(healthBarBackground);

	if (IS_EDIT_MODE)
	{
		auto button1Function = std::bind(File::SaveObjects, world->GetGameObjects(), "maps/showcase1.txt");
		Button* SaveButton = new Button("Save Map", glm::vec2(2, 82), 15, 10, glm::vec4(.7, 0, 0, 1), SCREEN_WIDTH, SCREEN_HEIGHT, world->GetGraphics()->GetWindow(), button1Function);
		buttonObjects.push_back(SaveButton);

	}
	do {
		timer->UpdateTimer();
		FPSText->SetText(timer->FPS().c_str());
		player->UpdateFromInputs(timer->DeltaTime(), IS_EDIT_MODE);
		if (IS_PLAY_MODE)
		{
			player->ApplyPlayerGravity(timer->DeltaTime());
			bool lastOnGround = player->IsOnGround();
			physics.Update(world->GetGameObjects(), player, lastOnGround, timer);

			PlayStepAudio(footstepSound, timer, player->GetVelocity(), player->IsOnGround(), &footSide);
		}
		else
		{
			editControls->ClickObject(world->GetGameObjects(), world->GetGraphics(), player, timer);
			if (editControls->GetClickedObject() != nullptr)
				Selected->SetText(editControls->GetClickedObject()->GetName().c_str());
			
			else
				Selected->SetText("");
			
			//Editor movement
			glm::vec3 newPos = player->GetPosition() + player->GetVelocity() * (float)timer->DeltaTime();
			player->SetDesiredPos(newPos);
			player->MoveToDesired();
			
			editControls->MoveClickedObject(pWindow, player, timer->DeltaTime());
			editControls->ScaleClickedObject(pWindow, player, timer->DeltaTime());
			editControls->GameObjectEditControls(pWindow);
			editControls->CheckConsoleInput();
		}
		world->GetGraphics()->RenderObjects(world->GetGraphics()->GetGraphicObjects(), textObjects, buttonObjects, quadObjects, player->GetProjectionMatrix(), player->GetViewMatrix());
		if(IS_EDIT_MODE)
			world->RenderHitBoxes(player->GetProjectionMatrix(), player->GetViewMatrix());

		glfwSwapBuffers(pWindow);
		glfwPollEvents();
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(pWindow) == 0);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}