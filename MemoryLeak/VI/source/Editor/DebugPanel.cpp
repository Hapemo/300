/*!*****************************************************************************
/*!*****************************************************************************
\file DebugPanel.cpp
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Group: Memory Leak Studios
\date 20-10-2022
\brief
This file contains function definations for a Debug Panel Editor system that displays debug infomation from logger
*******************************************************************************/
#include "DebugPanel.h"
#include <ECSManager.h>
/*!*****************************************************************************
\brief
	Initializes the Debug Panel editor

\return
None.
*******************************************************************************/
void DebugPanel::Init()
{
	loggerStr = Logger::GetInstance()->GetLoggerStr();
}
/*!*****************************************************************************
\brief
	Updates the Debug Panel editor

\return
None.
*******************************************************************************/
void DebugPanel::Update()
{
	if (!active)
		return;
	//loggerStr = Logger::GetInstance()->GetLoggerStr();
	Init();
	if (ImGui::Begin("Debug Logger"))
	{
		ImGui::BeginTabBar("DebugLogger");
		if (ImGui::BeginTabItem("All Logs "))
		{
			for (auto str : loggerStr)
			{
				ImGui::Text(str.second.c_str());
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Info Log "))
		{
			for (auto str : loggerStr)
			{
				if (str.first == Logger::E_LOGLEVEL::LOG_INFO)
					ImGui::Text(str.second.c_str());
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Debug Log "))
		{

			for (auto str : loggerStr)
			{
				if (str.first == Logger::E_LOGLEVEL::LOG_DEBUG)
					ImGui::Text(str.second.c_str());
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Warning Log "))
		{
			for (auto str : loggerStr)
			{
				if (str.first == Logger::E_LOGLEVEL::LOG_WARN)
					ImGui::Text(str.second.c_str());
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Error Log "))
		{
			for (auto str : loggerStr)
			{
				if (str.first == Logger::E_LOGLEVEL::LOG_ERROR)
					ImGui::Text(str.second.c_str());
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Custom Log "))
		{
			for (auto str : loggerStr)
			{
				if (str.first > Logger::E_LOGLEVEL::LOG_ASSERT)
					ImGui::Text(str.second.c_str());
			}
			ImGui::EndTabItem();
		}
	
		ImGui::Dummy({ 10,100 });
		ImGui::EndTabBar();
	}
	ImGui::End();

}
/*!*****************************************************************************
\brief
	Free the Debug Panel editor

\return
None.
*******************************************************************************/
void DebugPanel::Free()
{

}