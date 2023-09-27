/*!*************************************************************************
****
\file Performance.cpp
\author Han ChengWen, Lor Xaun Yun Michelle
\par DP email: c.han@digipen.edu, xaunyunmichelle.lor@digipen.edu
\date 22-9-2022
\brief
This file contains Performance window functions that display
the Frames of different system etc Graphics,physics.. and also total frame for 
whole execution.

The functions
- update
Contains main loop for the logic of Performance window, drawing Histograph,
Line graph as display setting.

- m_hovered
Return true if mouse is within window
****************************************************************************
***/
#include "Performance.h"
#include <cstdio>
#include "../../../_TOOLS/include/FPSManager.h"
#include "../../../_ENGINE/include/Debug/EnginePerformance.h"

float Performance::values[INTERVALS] = {0.f};
float Performance::graphicsValues[INTERVALS] = {0.f};
float Performance::physicsValues[INTERVALS] = {0.f};
float Performance::scriptingValues[INTERVALS] = { 0.f };
float Performance::resourceValues[INTERVALS] = { 0.f };
float Performance::gameStateValues[INTERVALS] = { 0.f };
float Performance::audioValues[INTERVALS] = { 0.f };
float Performance::loggerValues[INTERVALS] = { 0.f };
float Performance::editorValues[INTERVALS] = { 0.f };
//float Performance::audioValues[INTERVALS] = { 0.f };
//float Performance::min = 60.f;
int Performance::fpsCount = 0;
int Performance::sysCount = 0;

void Performance::init()
{
    //empty
}

void Performance::update()
{
    // Displays graphics % then physics % on the histogram
    //float arr[] = { gDisplayTemp, pDisplayTemp };

    static int values_offset = 0;
    if (FPSManager::trigger)
    {
        Performance::values[fpsCount++] = static_cast<float>(FPSManager::fps);
        //std::cout << static_cast<float>(FPSManager::fps) << std::endl;
        FPSManager::trigger = !FPSManager::trigger;
    }
    if (EnginePerformance::trigger)
    {
        //graphicsValues[sysCount] = EnginePerformance::FpsPercentage(EnginePerformance::systemDuration[0]);
        //physicsValues[sysCount] = EnginePerformance::FpsPercentage(EnginePerformance::systemDuration[1]);
        //scriptingValues[sysCount] = EnginePerformance::FpsPercentage(EnginePerformance::systemDuration[2]);
        //resourceValues[sysCount] = EnginePerformance::FpsPercentage(EnginePerformance::systemDuration[3]);
        //gameStateValues[sysCount] = EnginePerformance::FpsPercentage(EnginePerformance::systemDuration[4]);
        //audioValues[sysCount] = EnginePerformance::FpsPercentage(EnginePerformance::systemDuration[5]);
        //loggerValues[sysCount] = EnginePerformance::FpsPercentage(EnginePerformance::systemDuration[6]);

        std::map<std::string, double>::iterator it;
        for (it = EnginePerformance::systemT.begin(); it != EnginePerformance::systemT.end(); ++it)
        {
            if (it->first == "Graphics")
                graphicsValues[sysCount] = static_cast<float>(it->second);
            else if (it->first == "Physics")
                physicsValues[sysCount] = static_cast<float>(it->second);
            else if (it->first == "Scripting")
                scriptingValues[sysCount] = static_cast<float>(it->second);
            else if (it->first == "Resource")
                resourceValues[sysCount] = static_cast<float>(it->second);
            else if (it->first == "GameState")
                gameStateValues[sysCount] = static_cast<float>(it->second);
            else if (it->first == "Audio")
                audioValues[sysCount] = static_cast<float>(it->second);
            else if (it->first == "Logger")
                loggerValues[sysCount] = static_cast<float>(it->second);
            else if (it->first == "Editor")
                editorValues[sysCount] = static_cast<float>(it->second);
        }

        gDisplayTemp = Performance::graphicsValues[sysCount];
        pDisplayTemp = Performance::physicsValues[sysCount];
        sDisplayTemp = Performance::scriptingValues[sysCount];
        rDisplayTemp = Performance::resourceValues[sysCount];
        gsDisplayTemp = Performance::gameStateValues[sysCount];
        aDisplayTemp = Performance::audioValues[sysCount];
        lDisplayTemp = Performance::loggerValues[sysCount];
        eDisplayTemp = Performance::editorValues[sysCount];

        //EnginePerformance::trigger = !EnginePerformance::trigger;
        ++sysCount;
    }
    // Plots can display overlay texts
    // (in this example, we will display an average value)
    {
        char overlay[32];
        char graphicsOverlay[32];
        char physicsOverlay[32];
        char scriptingOverlay[32];
        //char resourceOverlay[32];
        //char gameStateOverlay[32];
        char audioOverlay[32];
        //char loggerOverlay[32];
        char editorOverlay[32];
        //char audioOverlay[32];
        //Print and display total FPS
        //Print and display total FPS
        sprintf_s(overlay, "FPS %.2f", values[fpsCount]);
        ImGui::PlotLines("Total FPS", values, IM_ARRAYSIZE(values), values_offset, overlay, -10.f, 200.0f, ImVec2(0, 300.0f));
        //Print and display graphics ms / total ms
        if (gDisplayTemp > 0)
        {
            sprintf_s(graphicsOverlay, "GRAPHICS/DT %.2f%%", gDisplayTemp);
            ImGui::PlotLines("Graphics/dt", graphicsValues, IM_ARRAYSIZE(graphicsValues), values_offset, graphicsOverlay, -20.0f, 150.0f, ImVec2(0.f, 70.0f));
        }
        //Print and display physics ms / total ms
        if (pDisplayTemp > 0)
        {
            sprintf_s(physicsOverlay, "PHYSICS/DT %.2f%%", pDisplayTemp);
            ImGui::PlotLines("Physics/dt", physicsValues, IM_ARRAYSIZE(physicsValues), values_offset, physicsOverlay, -20.0f, 150.0f, ImVec2(0.f, 70.0f));
        }
        //Print and display scripting ms / total ms
        if (sDisplayTemp > 0)
        {
            sprintf_s(scriptingOverlay, "SCRIPTING/DT %.2f%%", sDisplayTemp);
            ImGui::PlotLines("Scripting/dt", scriptingValues, IM_ARRAYSIZE(scriptingValues), values_offset, scriptingOverlay, -20.0f, 150.0f, ImVec2(0.f, 70.0f));
        }
        //Print and display audio ms / total ms
        if (aDisplayTemp > 0)
        {
            sprintf_s(audioOverlay, "AUDIO/DT %.2f%%", aDisplayTemp);
            ImGui::PlotLines("Audio/dt", audioValues, IM_ARRAYSIZE(audioValues), values_offset, audioOverlay, -20.0f, 150.0f, ImVec2(0.f, 70.0f));
        }
        if (eDisplayTemp > 0)
        {
            sprintf_s(editorOverlay, "EDITOR/DT %.2f%%", eDisplayTemp);
            ImGui::PlotLines("Editor/dt", editorValues, IM_ARRAYSIZE(editorValues), values_offset, editorOverlay, -20.0f, 150.0f, ImVec2(0.f, 70.0f));
        }
    }

    if (fpsCount == INTERVALS)
        fpsCount = 0;

    if (sysCount == INTERVALS)
        sysCount = 0;
}

bool Performance::m_Hovered()
{
    return scene_m_Hovered;
}
