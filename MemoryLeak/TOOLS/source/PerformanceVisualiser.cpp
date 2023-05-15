/*!*****************************************************************************
\file PerformanceVisualiser.cpp
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 26-09-2022
\brief
This file contains the function definitions of the class PerformanceVisualiser.
The PerformanceVisualiser class tracks and measure the performance speed for
different systems in the engine and displays them in percentage.
*******************************************************************************/

#include "PerformanceVisualiser.h"

/*!*****************************************************************************
\brief
Register and start tracking the system usage.

\param _systemName
Unique name identifier of the system to be tracked.
*******************************************************************************/
void PerformanceVisualiser::StartSystemTracking(std::string _systemName)
{
	for (std::pair<std::string, std::pair<double, std::pair<TIMEPOINT, TIMEPOINT>> > &system : PerformanceVisualiser::mSystemPerformance) {
		if (system.first == _systemName) {
			TIMEPOINT startTimepoint = std::chrono::high_resolution_clock::now();
			system.second.second.first = startTimepoint;
			return;
		}
	}
	TIMEPOINT startTimepoint = std::chrono::high_resolution_clock::now();
	PerformanceVisualiser::mSystemPerformance.push_back({ _systemName, { 0, { startTimepoint, startTimepoint } } });
}

/*!*****************************************************************************
\brief
End the tracking of the system usage. Calculates the time taken and stores them
in the member vector mSystemPerformance.

\param _systemName
Unique name identifier of the system.

\return
Returns a double of the system performance in millisec.
*******************************************************************************/
double PerformanceVisualiser::StopSystemTracking(std::string _systemName)
{
	for (std::pair<std::string, std::pair<double, std::pair<TIMEPOINT, TIMEPOINT>> > &system : PerformanceVisualiser::mSystemPerformance) {
		if (system.first == _systemName) {
			std::pair<TIMEPOINT, TIMEPOINT>* systemTrack = &(system.second.second);
			(*systemTrack).second = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>((*systemTrack).first).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>((*systemTrack).second).time_since_epoch().count();

			long long duration = end - start;
			double millisec = 0.0;
			millisec = duration * 0.001;
			double* seconds = &(system.second.first);
			*seconds = millisec / 1000;
			
			return millisec;
		}
	}
	return 0.0;
}

/*!*****************************************************************************
\brief
Gets a specific system's performance.

\param _systemName
Unique name identifier of the system.

\return
Returns a double of the system performance in millisec.
*******************************************************************************/
double PerformanceVisualiser::GetPerformance(std::string _systemName) {
	for (const std::pair<std::string, std::pair<double, std::pair<TIMEPOINT, TIMEPOINT>> > &system : PerformanceVisualiser::mSystemPerformance)
		if (system.first == _systemName)
			return system.second.first;
	return 0.0;
}

/*!*****************************************************************************
\brief
Measures and gets a string of all the systems' performances in percentages.

\return
Returns a string populated with all the system performance in percentages.
*******************************************************************************/
std::string PerformanceVisualiser::GetPerformances() {
	bool firstSystem = true;
	double totalUsed = 0;
	double MainLoop = 0;
	std::string result = "(";

	for (const std::pair<std::string, std::pair<double, std::pair<TIMEPOINT, TIMEPOINT>> >& system : PerformanceVisualiser::mSystemPerformance)
		if (system.first == "MainLoop") MainLoop = system.second.first;

	for (const std::pair<std::string, std::pair<double, std::pair<TIMEPOINT, TIMEPOINT>> > &system : PerformanceVisualiser::mSystemPerformance) {
		std::string systemName = system.first;
		if (systemName != "MainLoop") if (firstSystem) firstSystem = false; else result += ", ";
		double systemSeconds = system.second.first;
		double systemPercent = (systemSeconds / MainLoop) * 100;
		if (systemName != "MainLoop") totalUsed += systemPercent;
		if (systemName != "MainLoop") result += systemName + ": " + std::to_string(systemPercent) + "%";
	}
	//if (!firstSystem) result += ", ";
	//result += "Others: " + std::to_string(100.0 - totalUsed) + "%";
	result += ")";

	return result;
}

/*!*****************************************************************************
\brief
Measures and returns a vector of all the systems' performances in percentages.

\return
Returns a vector populated with all the system performance in percentages.
*******************************************************************************/
std::vector < std::pair < std::string, double >> PerformanceVisualiser::GetPerformanceVec() {
	std::vector < std::pair < std::string, double >> result{};
	bool firstSystem = true;
	double totalUsed = 0;
	double MainLoop = 0;

	for (const std::pair<std::string, std::pair<double, std::pair<TIMEPOINT, TIMEPOINT>> >& system : PerformanceVisualiser::mSystemPerformance)
		if (system.first == "MainLoop") MainLoop = system.second.first;

	for (const std::pair<std::string, std::pair<double, std::pair<TIMEPOINT, TIMEPOINT>> >& system : PerformanceVisualiser::mSystemPerformance) {
		std::string systemName = system.first;
		if (systemName != "MainLoop") if (firstSystem) firstSystem = false; //else result += ", ";
		double systemSeconds = system.second.first;
		double systemPercent = (systemSeconds / MainLoop) * 100;
		if (systemName != "MainLoop") totalUsed += systemPercent;
		if (systemName != "MainLoop") {
			result.push_back(std::make_pair(systemName, systemPercent));
			//result.push_back(std::make_pair(systemName, system.second.first));
		}
	}

	return result;
}
