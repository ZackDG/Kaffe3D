#include "SoundSource.h"

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
OpenAL_ErrorCheck(FUNCTION_CALL)\

SoundSource::SoundSource(const char* path, glm::vec3 pos)
{
	const char* prefix = "sounds/";
	std::string finalPath = prefix;
	finalPath += path;
	if (!SoundFile.load(finalPath.c_str()))
	{
		std::cout << "Failed to load sound file" << std::endl;
	}

	int bitDepth = SoundFile.getBitDepth();
	ALenum format;
	if (bitDepth == 16)
	{
		if (SoundFile.isStereo())
		{
			format = AL_FORMAT_STEREO16;
		}
		else
		{
			format = AL_FORMAT_MONO16;
		}
	}
	else if (bitDepth == 8)
	{
		if (SoundFile.isStereo())
		{
			format = AL_FORMAT_STEREO8;
		}
		else
		{
			format = AL_FORMAT_MONO8;
		}
	}
	else
	{
		format = -1;
	}
	if (format == -1)
	{
		std::cout << "Error setting audio format";
	}
	SoundFile.writePCMToBuffer(monoPCMDataBytes);

	//Generate buffers, 
	//alGetError();	//Clear error code
	alec(alGenBuffers(1, &SoundBuffer));
	alec(alBufferData(SoundBuffer, format, monoPCMDataBytes.data(), monoPCMDataBytes.size(), SoundFile.getSampleRate()));

	// Set Source Info
	alec(alGenSources(1, &Source));
	alec(alSource3f(Source, AL_POSITION, pos.x, pos.y, pos.z));
	alec(alSource3f(Source, AL_VELOCITY, 0.0f, 0.0f, 0.0f));
	alec(alSourcef(Source, AL_PITCH, 1.0f));
	alec(alSourcef(Source, AL_GAIN, 1.0f));
	alec(alSourcei(Source, AL_LOOPING, AL_FALSE));
	alec(alSourcei(Source, AL_BUFFER, SoundBuffer));
}

void SoundSource::SetPosition(glm::vec3 pos)
{
	alec(alSource3f(Source, AL_POSITION, pos.x, pos.y, pos.z));
}

void SoundSource::SetVelocity(glm::vec3 vel)
{
	alec(alSource3f(Source, AL_VELOCITY, vel.x, vel.y, vel.z));
}

SoundSource::~SoundSource()
{
	alec(alDeleteSources(1, &Source));
	alec(alDeleteBuffers(1, &SoundBuffer));
}

ALint SoundSource::GetSourceState()
{
	alec(alGetSourcei(Source, AL_SOURCE_STATE, &SourceState));
	return SourceState;
}

void SoundSource::Play()
{
	alec(alSourcePlay(Source));
}

void SoundSource::Stop()
{
	alec(alSourceStop(Source));
}

void SoundSource::Rewind()
{
	alec(alSourceRewind(Source));
}
