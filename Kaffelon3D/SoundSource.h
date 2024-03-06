#pragma once
#include "AudioFile.h"
#include "openal/al.h"
#include <string>
#include <glm/glm.hpp>


class SoundSource
{
public:
	SoundSource(const char* path, glm::vec3 pos);
	void SetPosition(glm::vec3 pos);
	void SetVelocity(glm::vec3 vel);
	~SoundSource();
	ALint GetSourceState();
	void Play();
	void Stop();
	void Rewind();

private:
	std::vector<uint8_t> monoPCMDataBytes;
	AudioFile<float> SoundFile;
	ALuint SoundBuffer = 0;
	ALuint Source = 0;
	ALint SourceState = 0;
};

