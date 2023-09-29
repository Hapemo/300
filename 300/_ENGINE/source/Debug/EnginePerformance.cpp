/*!*************************************************************************
****
\file EnginePerformance.cpp
\author Lor Xaun Yun Michelle
\par DP email: xaunyunmichelle.lor\@digipen.edu
\date 28-9-2023
\brief
This file contains the logic to track and collect data for
the imGUI to show the engine performance.
This file contiains the functions:
- UpdateSystemMs
This function calculate the total time obtained from the systems.
Divide it according to the delta time of the engine the multiply
by 100 to get the percentage of system's performance / total
engine performance
- StoreTime
Store the time, this is used for Editor performance for now
as the recording of time had to be split up due to the editor's
performance function being called halfway through the editor update
- TotalTime
This is the follow up helper function from StoreTime, it adds the
stored time to the time accumulated in the systemT[str]
- StartTrack
Placed at the start of the update loop to get start time
- EndTrack
Placed at the end of the update loop to get end time
****************************************************************************
***/
#include "Debug/EnginePerformance.h"
#include "FPSManager.h"
#include "Debug/LogException.h"
#include <string>
#include "glfw3.h"

int EnginePerformance::sysCount = 0;
std::map<std::string, double> EnginePerformance::systemT{};
bool EnginePerformance::trigger = false;
double EnginePerformance::storedTime = 0.0;

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

void EnginePerformance::StoreTime(std::string str)
{
	storedTime = systemT[str];
	systemT[str] = 0.0;
}

void EnginePerformance::TotalTime(std::string str)
{
	systemT[str] += storedTime;
}
