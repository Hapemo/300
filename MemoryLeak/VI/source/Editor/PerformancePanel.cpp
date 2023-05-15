/*!*****************************************************************************
/*!*****************************************************************************
\file PerformancePanel.cpp
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Group: Memory Leak Studios
\date 20-11-2022
\brief
This file contains function definations for a Performance Panel Editor system that displays Performance infomation 
*******************************************************************************/
#include "PerformancePanel.h"
#include <ECSManager.h>
#define MaxSystem 16
#define MaxData 16
/*!*****************************************************************************
\brief
	Initializes the Performance Panel editor

\return
None.
*******************************************************************************/
void PerformancePanel::Init()
{
	updateTime = 10;
	for (int i = 0; i < MaxSystem; i++)
	{
		std::deque<float> list;
		for (int d = 0; d < MaxData; d++)
		{
			list.push_front(0.f);
		}
		system.push_back(list);
	}
}
/*!*****************************************************************************
\brief
	Updates the Performance Panel editor

\return
None.
*******************************************************************************/
void PerformancePanel::Update()
{
	if (ImGui::Begin("Performance Visualizer"))
	{
		ImGui::BeginTabBar("PerformanceVisualizer");
		if (ImGui::BeginTabItem("Overall"))
		{
			static float arr[MaxSystem];
			int seconds = (int)(ImGui::GetTime() * 60);
			if (seconds % updateTime == 0)
			{
				performance = PerformanceVisualiser::GetInstance()->GetPerformanceVec();
				for (int i = 0; i < performance.size(); i++)
				{
					if (seconds % (updateTime*2) == 0)
						arr[i] = (float)performance[i].second;
					system[i].push_front((float)performance[i].second);
				}
			}
			ImGui::PlotHistogram("ALL  ", arr, IM_ARRAYSIZE(arr), 0, NULL, 0.0f, 100.0f, ImVec2(0, 80.0f));
			ImGui::SameLine();
			ImGui::PushItemWidth(70);
			ImGui::InputInt("Upate", &updateTime, 2, 5);
			ImGui::PopItemWidth();
			updateTime = updateTime < 1 ? 2 : (updateTime>30?30:updateTime);
			for (int i = 0; i < performance.size(); i++)
			{
				static float data[MaxData];
				for (int d = 0; d < MaxData; d++)
				{
					data[MaxData - 1 - d] = system[i][d];
				}
				char avg[100];
				sprintf(avg, "%.2f %%", arr[i]);
				ImGui::PlotLines(performance[i].first.c_str(), data, IM_ARRAYSIZE(data), 0, avg, 0.f, 100.f, ImVec2(0, 80));
			}
		}
		ImGui::Dummy({ 10,100 });
		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();
	ImGui::End();

}
/*!*****************************************************************************
\brief
	Free the Performance Panel editor

\return
None.
*******************************************************************************/
void PerformancePanel::Free()
{

}