/*!*************************************************************************
****
\file Script.h
\author(s) Lor Xaun Yun Michelle
\par DP email:
xaunyunmichelle.lor\@digipen.edu
\date 3-11-2023
\brief
This file contains a function InspectScript that will be called under 
ImGui's inspect.
- InspectScript
Aid in rendering the lua reflection variables in the ImGui.
****************************************************************************
***/
#pragma once
#include "ScriptingSystem.h"

void InspectScript(Script* script);
