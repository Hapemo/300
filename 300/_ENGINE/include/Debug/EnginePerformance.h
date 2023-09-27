#pragma once
#include <string>
#include <array>
#include <map>

class EnginePerformance
{
public:
	// call glfwGetTime() function
	//static void GetTime(double& time);
	// update systems ms
	static void UpdateSystemMs(std::string str);
	static void StoreTime(std::string str);
	static void TotalTime(std::string str);
	//static float FpsPercentage(const double& systemFps);

	static void StartTrack(std::string str);
	static void EndTrack(std::string str);

	static int sysCount;
	//static std::array<double, 7> systemDuration;
	static std::map<std::string, double> systemT;
	static bool trigger;
	static double storedTime;
};
