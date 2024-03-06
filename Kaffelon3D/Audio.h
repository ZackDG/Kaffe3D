#pragma once

#include "AudioFile.h"
#include "openal/al.h"
#include "openal/alc.h"
#include "SoundSource.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class Audio
{
public:
	Audio();
	~Audio();
	SoundSource* CreateSoundSource(const char* path, glm::vec3 pos);
	void UpdatePlayerPosition(glm::vec3 pos);
	void UpdatePlayerVelocity(glm::vec3 vel);
	void UpdatePlayerAngle(glm::vec3 direction, glm::vec3 up);
	void PlayAudio();
private:
	std::vector<SoundSource*> SoundSources;
	ALCdevice* Device;
	ALCcontext* Context;
};