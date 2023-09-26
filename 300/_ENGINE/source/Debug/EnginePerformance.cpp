#include "../Debug/EnginePerformance.h"
#include "../../../_TOOLS/include/FPSManager.h"
#include "../Debug/LogException.h"
#include <string>
#include "glfw3.h"

int EnginePerformance::sysCount = 0;
//std::array<double, 7> EnginePerformance::systemDuration{};
std::map<std::string, double> EnginePerformance::systemT{};
bool EnginePerformance::trigger = false;

// update systems ms
void EnginePerformance::UpdateSystemMs(std::string str)
{
	std::map<std::string, double>::iterator it;
	for (it = systemT.begin(); it != systemT.end(); ++it)
	{
		if (it->first == str)
		{
			if (FPSManager::dt != 0)
			{
				it->second /= (FPSManager::dt / 0.001);
				it->second *= 100.f;
			}
			break;
		}
	}
	EnginePerformance::trigger = true;
}

void EnginePerformance::StartTrack(std::string str)
{
	systemT[str] = glfwGetTime() / 0.001;
}
void EnginePerformance::EndTrack(std::string str)
{
	systemT[str] = (glfwGetTime() / 0.001) - systemT[str];
}
