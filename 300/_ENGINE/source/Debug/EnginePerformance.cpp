#include "../Debug/EnginePerformance.h"
#include "../../../_TOOLS/include/FPSManager.h"
#include "../Debug/LogException.h"
#include <string>
#include "glfw3.h"

int EnginePerformance::sysCount = 0;
std::array<double, 7> EnginePerformance::systemDuration{};
bool EnginePerformance::trigger = false;

// call glfwGetTime() function
void EnginePerformance::GetTime(double& time)
{
	time = glfwGetTime();
}

// update systems ms
void EnginePerformance::UpdateSystemMs(std::string systemName,
	const double& startTime,
	const double& endTime)
{
	double elapsedTime = endTime - startTime;

	// update after awhile
	if (sysCount > 100)
	{
		if (systemName == "Graphics")
			systemDuration[0] = elapsedTime / 0.001;
		else if (systemName == "Physics")
			systemDuration[1] = elapsedTime / 0.001;
		else if (systemName == "Scripting")
			systemDuration[2] = elapsedTime / 0.001;
		else if (systemName == "Resource")
			systemDuration[3] = elapsedTime / 0.001;
		else if (systemName == "Gamestate")
			systemDuration[4] = elapsedTime / 0.001;
		else if (systemName == "Audio")
			systemDuration[5] = elapsedTime / 0.001;
		else if (systemName == "Logger")
			systemDuration[6] = elapsedTime / 0.001;

		trigger = !trigger;
		sysCount = 0;
	}
	++sysCount;
}

float EnginePerformance::FpsPercentage(const double& systemFps)
{
	//Compare ms of update loop
	//Uncomment this line of code to test exception log
	//fps = 0.0;
	//if (FPSManager::elapsedTimeMs == 0.0)
	//{
	//	PEXCEPTIONFOUND("ms of 0.0 is being used.");
	//}
	if (FPSManager::elapsedTimeMs == 0)
		return 0.f;
	return (static_cast<float>(systemFps / FPSManager::elapsedTimeMs)) * 100.f;
}
