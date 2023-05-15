/*!*****************************************************************************
\file RenderProps.h
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 22-10-2022
\brief
This file contains information related to the rendering of objects.
*******************************************************************************/
#pragma once
//define number of triangles for circle
constexpr int CIRCLE_SLICES = 34;
//modifier for vector.reserve() 
constexpr float MODIFIER = 0.05f;
constexpr size_t NO_OF_OBJECTS = 10000;
constexpr size_t VERTICES_PER_OBJECT = 4;
constexpr size_t INDICES_PER_OBJECT = 6;
//maximum used texture units in 1 draw call
constexpr size_t TEXTURES_PER_DRAW = 16;

constexpr int MAX_SCENE_LAYERS = 16;
constexpr int SHADOW_SCENE_LAYER = 1;
constexpr int MAX_LAYERS_PER_SCENE = 1024;