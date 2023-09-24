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

float Performance::values[INTERVALS] = {0.f};
float Performance::graphicsValues[INTERVALS] = {0.f};
float Performance::physicsValues[INTERVALS] = {0.f};
float Performance::scriptingValues[INTERVALS] = { 0.f };
//float Performance::audioValues[INTERVALS] = { 0.f };
float Performance::min = 60.f;
int Performance::fpsCount = 0;

void Performance::init()
{
    //empty
}

void Performance::update()
{
    static int values_offset = 0;
    if (FPSManager::trigger)
    {
        Performance::values[fpsCount] = static_cast<float>(FPSManager::fps);
        //std::cout << static_cast<float>(FPSManager::fps) << std::endl;
        ++fpsCount;
        FPSManager::trigger = !FPSManager::trigger;
    }
    // Plots can display overlay texts
    // (in this example, we will display an average value)
    {
        char overlay[32];
        char graphicsOverlay[32];
        char physicsOverlay[32];
        char scriptingOverlay[32];
        //char audioOverlay[32];
        //Print and display total FPS
        //Print and display total FPS
        sprintf_s(overlay, "FPS %.4f, MIN FPS %.4f", values[fpsCount], min);
        ImGui::PlotLines("Total FPS", values, IM_ARRAYSIZE(values), values_offset, overlay, -10.f, 100.0f, ImVec2(0, 300.0f));
    }

    if (fpsCount == INTERVALS)
        fpsCount = 0;
}

bool Performance::m_Hovered()
{
    return scene_m_Hovered;
}
