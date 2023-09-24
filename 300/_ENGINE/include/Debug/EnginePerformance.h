#pragma once
#include <string>
#include "../../../_EDITOR/include/Editor/Performance.h"

class EnginePerformance
{
	// call glfwGetTime() function
	void GetTime(double& time);
	// update systems ms
	void UpdateSystemMs(std::string systemName,
		const double& startTime,
		const double& endTime);
	float FpsPercentage(const double& systemFps);
};
