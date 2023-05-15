/*!*****************************************************************************
\file GLAllocator.h
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 20-09-2022
\brief
This file contains function declarations for GLAllocator, which allocates memory
to a Vertex Buffer Object contained by a Vertex Array Object.
*******************************************************************************/
#pragma once
#include "Vertex.h"

/*!*****************************************************************************
\brief
GLAllocator class that provides functions to interact with OpenGL.
*******************************************************************************/
class GLAllocator
{
public:
	/*!*****************************************************************************
	\brief
	Constructor for Animator class.

	\param size_t _noOfObjects
	Number of objects to allocate memory for.

	\param size_t _verticesPerObject
	Number of vertices per object.

	\param size_t _indicesPerObject
	Number of indices per object.
	*******************************************************************************/
	GLAllocator(size_t _noOfObjects, size_t _verticesPerObject, size_t _indicesPerObject);
	/*!*****************************************************************************
	\brief
	Binds the VAO to the OpenGL context.
	*******************************************************************************/
	void BindVAO();
	/*!*****************************************************************************
	\brief
	Binds the VBO to the OpenGL context.
	*******************************************************************************/
	void BindVBO();
	/*!*****************************************************************************
	\brief
	Unbinds the VAO from the OpenGL context.
	*******************************************************************************/
	void UnbindVAO();
	/*!*****************************************************************************
	\brief
	Unbinds the VBO from the OpenGL context.
	*******************************************************************************/
	void UnbindVBO();
	GLuint mvaoid;
	GLuint mvboid;
	GLuint meboid;
private:
};

