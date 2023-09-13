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

float Performance::values[INTERVALS] = {0.f};
float Performance::graphicsValues[INTERVALS] = {0.f};
float Performance::physicsValues[INTERVALS] = {0.f};
float Performance::scriptingValues[INTERVALS] = { 0.f };
//float Performance::audioValues[INTERVALS] = { 0.f };
float Performance::min = 60.f;

void Performance::init()
{
    //empty
}

void Performance::update()
{
   

    // Plots can display overlay texts
    // (in this example, we will display an average value)
    {
        char overlay[32];
        char graphicsOverlay[32];
        char physicsOverlay[32];
        char scriptingOverlay[32];
        //char audioOverlay[32];
        //Print and display total FPS
    }
}

bool Performance::m_Hovered()
{
    return scene_m_Hovered;
}
