/*!*****************************************************************************
\file PerformanceVisualiser.h
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 26-09-2022
\brief
This file contains the function declarations of the class PerformanceVisualiser.
The PerformanceVisualiser class tracks and measure the performance speed for
different systems in the engine and displays them in percentage.
*******************************************************************************/

#pragma once
#include "Singleton.h"
#include <chrono>
#include <string>
#include <vector>

#define TIMEPOINT std::chrono::time_point<std::chrono::high_resolution_clock>
#define GET_SYSTEMS_PERFORMANCES() PerformanceVisualiser::GetInstance()->GetPerformances();
#define GET_SYSTEM_PERFORMANCE(_systemName) PerformanceVisualiser::GetInstance()->GetPerformance(_systemName);
#define TRACK_PERFORMANCE(_systemName) PerformanceVisualiser::GetInstance()->StartSystemTracking(_systemName);
#define END_TRACK(_systemName) PerformanceVisualiser::GetInstance()->StopSystemTracking(_systemName);

class PerformanceVisualiser : public Singleton<PerformanceVisualiser>
{
public:
	/*!*****************************************************************************
	\brief
	Default constructor and destructor.
	*******************************************************************************/
	PerformanceVisualiser() = default;
	~PerformanceVisualiser() = default;

	/*!*****************************************************************************
	\brief
	Delete copy constructor.
	*******************************************************************************/
	PerformanceVisualiser(const PerformanceVisualiser&) = delete;
	const PerformanceVisualiser& operator=(const PerformanceVisualiser&) = delete;

	/*!*****************************************************************************
	\brief
	Register and start tracking the system usage.

	\param _systemName
	Unique name identifier of the system to be tracked.
	*******************************************************************************/
	void StartSystemTracking(std::string _systemName);

	/*!*****************************************************************************
	\brief
	End the tracking of the system usage. Calculates the time taken and stores them
	in the member vector mSystemPerformance.

	\param _systemName
	Unique name identifier of the system.

	\return
	Returns a double of the system performance in millisec.
	*******************************************************************************/
	double StopSystemTracking(std::string _systemName);

	/*!*****************************************************************************
	\brief
	Gets a specific system's performance.

	\param _systemName
	Unique name identifier of the system.

	\return
	Returns a double of the system performance in millisec.
	*******************************************************************************/
	double GetPerformance(std::string _systemName);

	/*!*****************************************************************************
	\brief
	Measures and gets a string of all the systems' performances in percentages.

	\return
	Returns a string populated with all the system performance in percentages.
	*******************************************************************************/
	std::string GetPerformances();

	/*!*****************************************************************************
	\brief
	Measures and returns a vector of all the systems' performances in percentages.

	\return
	Returns a vector populated with all the system performance in percentages.
	*******************************************************************************/
	std::vector < std::pair < std::string, double >> GetPerformanceVec();
private:
	/*!*****************************************************************************
	\brief
	Member vector storing data of the different system performances.
	*******************************************************************************/
	std::vector<std::pair<std::string, std::pair<double, std::pair<TIMEPOINT, TIMEPOINT>>>> mSystemPerformance;
};
