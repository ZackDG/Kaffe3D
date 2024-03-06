#include "Audio.h"


#define OpenAL_ErrorCheck(message)\
{\
	ALenum error = alGetError();\
	if(error != AL_NO_ERROR)\
	{\
		std::cout << "OpenAL Error: " << error << " with call for " << #message << std::endl;\
	}\
}

#define alec(FUNCTION_CALL)\
FUNCTION_CALL;\
OpenAL_ErrorCheck(FUNCTION_CALL)

Audio::Audio()
{
	Device = alcOpenDevice(NULL);
	if (!Device)
	{
		std::cout << "Failed to make OpenAL device" << std::endl;
	}

	Context = alcCreateContext(Device, NULL);


	if (!alcMakeContextCurrent(Context))
	{
		std::cout << "Failed to make OpenAL context" << std::endl;
	}

	// Create listener in 3D space (Player)
	alec(alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f));
	alec(alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f));
	ALfloat forwardAndUpVectors[] = {
		/*forward*/ 1.0f, 0.0f, 0.0f,
		/*up*/		0.0f, 1.0f, 0.0f
	};


	alec(alListenerfv(AL_ORIENTATION, forwardAndUpVectors));
}

Audio::~Audio()
{
	alec(alcMakeContextCurrent(nullptr));
	alec(alcDestroyContext(Context));
	alec(alcCloseDevice(Device));
}


SoundSource* Audio::CreateSoundSource(const char* path, glm::vec3 pos)
{
	SoundSource* newSound = new SoundSource(path, pos);
	SoundSources.push_back(newSound);
	return newSound;
}

void Audio::UpdatePlayerPosition(glm::vec3 pos)
{
	alec(alListener3f(AL_POSITION, pos.x, pos.y, pos.z));
}

void Audio::UpdatePlayerVelocity(glm::vec3 vel)
{
	alec(alListener3f(AL_VELOCITY, vel.x, vel.y, vel.z));
}

void Audio::UpdatePlayerAngle(glm::vec3 direction, glm::vec3 up)
{
	ALfloat forwardAndUpVectors[] = {
		/*forward*/ direction.x, direction.y, direction.z,
		/*up*/		up.x, up.y, up.z
	};


	alec(alListenerfv(AL_ORIENTATION, forwardAndUpVectors));
}

void Audio::PlayAudio()
{

}
