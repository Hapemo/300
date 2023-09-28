/*!*************************************************************************
****
\file EnginePerformance.h
\author Lor Xaun Yun Michelle
\par DP email: xaunyunmichelle.lor\@digipen.edu
\date 28-9-2023
\brief
This file contains the logic to track and collect data for
the imGUI to show the engine performance.
This file contiains the functions:
- UpdateSystemMs, StoreTime, TotalTime, StartTrack, EndTrack
****************************************************************************
***/
#pragma once
#include <string>
#include <array>
#include <map>

class EnginePerformance
{
public:
	// update systems ms
	static void UpdateSystemMs(std::string str);
	static void StoreTime(std::string str);
	static void TotalTime(std::string str);

	static void StartTrack(std::string str);
	static void EndTrack(std::string str);

	static int sysCount;
	static std::map<std::string, double> systemT;
	static bool trigger;
	static double storedTime;
};
