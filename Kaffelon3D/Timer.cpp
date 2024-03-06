#include "Timer.h"

Timer::Timer()
{
	LastTime = std::chrono::system_clock::now();
	CurrentTime = std::chrono::system_clock::now();
}

void Timer::UpdateTimer()
{
	LastTime = CurrentTime;
	CurrentTime = std::chrono::system_clock::now();
}

double Timer::DeltaTime()
{
	std::chrono::duration<double> diff = CurrentTime - LastTime;
	return diff.count();
}

std::string Timer::FPS()
{
	// Measure speed
	nbFrames++;
	std::chrono::duration<double> diff = CurrentTime - LastFPSTime;
	if (diff.count() >= 1) {
		FramesDisplay = nbFrames;
		nbFrames = 0;
		LastFPSTime = CurrentTime;
	}
	return std::to_string(1000.0 / double(FramesDisplay)) + "ms/frame " + "FPS: " + std::to_string(FramesDisplay);
}

bool Timer::Interval(float time)
{
	if (Intervals.count(time) == 0)
	{
		Intervals.insert(std::pair<float, std::chrono::time_point<std::chrono::system_clock>>(time, std::chrono::system_clock::now()));
	}
	else
	{
		std::chrono::duration<float> diff = std::chrono::system_clock::now() - Intervals[time];
		if (diff.count() > time)
		{
			Intervals[time] = std::chrono::system_clock::now();
			return true;
		}
	}
	return false;
}

void Timer::UpdateInterval(float time)
{
	if (Intervals.count(time) == 0)
		Intervals[time] = std::chrono::system_clock::now();
}
