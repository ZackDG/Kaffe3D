#pragma once
#include <chrono>
#include <string>
#include <map>
class Timer
{
public:
	Timer();
	void UpdateTimer();
	double DeltaTime();
	std::string FPS();
	bool Interval(float time);
	void UpdateInterval(float time);
	std::chrono::time_point<std::chrono::system_clock> GetTime() { return CurrentTime; }
private:
	std::chrono::time_point<std::chrono::system_clock> LastTime;
	// Tracks time of last FPS display
	std::chrono::time_point<std::chrono::system_clock> LastFPSTime;
	std::chrono::time_point<std::chrono::system_clock> CurrentTime;
	std::map <float, std::chrono::time_point<std::chrono::system_clock>> Intervals;


	int FramesDisplay = 0;
	int nbFrames = 0;
};

