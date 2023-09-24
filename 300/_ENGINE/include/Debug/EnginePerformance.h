#pragma once
#include <string>
#include <array>

class EnginePerformance
{
public:
	// call glfwGetTime() function
	static void GetTime(double& time);
	// update systems ms
	static void UpdateSystemMs(std::string systemName,
		const double& startTime,
		const double& endTime);
	static float FpsPercentage(const double& systemFps);

	static int sysCount;
	static std::array<double, 7> systemDuration;
	static bool trigger;
};
