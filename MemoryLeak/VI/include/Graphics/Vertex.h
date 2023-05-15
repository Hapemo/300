/*!*****************************************************************************
\file Vertex.h
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 20-09-2022
\brief
This file contains a struct Vertex that will be used in an array of structs
to pass data into OpenGL's VBO. As such, it contains some variables of type
glm::vec.
*******************************************************************************/
#pragma once
#include <pch.h>
#include <glm/glm.hpp>

/*!*****************************************************************************
\brief
A struct containing the attributes stored in a single Vertex object.
*******************************************************************************/
struct Vertex
{
	Vertex() = default;
	glm::vec3 position;
	glm::vec4 color;
	glm::vec2 texCoords;
	float texID;
};
