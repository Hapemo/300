/*!*****************************************************************************
\file GLShader.h
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 20-09-2022
\brief
This file contains function declarations to encapsulate a OpenGL shader object.
*******************************************************************************/
#pragma once
#include "pch.h"
#include "Logger.h"

/*!*****************************************************************************
\brief
GLShader class that emulates and simplifies interation with OpenGL shaders.
*******************************************************************************/
class GLShader
{
public:
	/*!*****************************************************************************
	\brief
	Constructor for GLShader class.

	\param _vertexShader
	File path to the vertex shader source code.

	\param _fragmentShader
	File path to the fragment shader source code.
	*******************************************************************************/
	GLShader(const std::string& _vertexShader, const std::string& _fragmentShader);

	/*!*****************************************************************************
	\brief
	Destructor for GLShader class.
	*******************************************************************************/
	~GLShader();

	/*!*****************************************************************************
	\brief
	Compile the vertex shader's code.
	*******************************************************************************/
	void CompileVertexShader();

	/*!*****************************************************************************
	\brief
	Compile the fragment shader's code.
	*******************************************************************************/
	void CompileFragmentShader();

	/*!*****************************************************************************
	\brief
	Compile both vertex and fragment shader's code.
	*******************************************************************************/
	void CompileShaders();

	/*!*****************************************************************************
	\brief
	Links vertex and fragment shaders.
	*******************************************************************************/
	void LinkShaders();

	/*!*****************************************************************************
	\brief
	Compile and link both vertex and fragment shader's code.
	*******************************************************************************/
	void CompileLinkShaders();

	/*!*****************************************************************************
	\brief
	Checks if vertex and fragment shaders are compiled and linked correctly, will
	throw an exception otherwise.
	*******************************************************************************/
	void Validate();

	/*!*****************************************************************************
	\brief
	Binds the shader program to the OpenGL context
	*******************************************************************************/
	void Bind();

	/*!*****************************************************************************
	\brief
	Unbinds the shader program to the OpenGL context
	*******************************************************************************/
	void Unbind();

	/*!*****************************************************************************
	\brief
	Register a uniform and its location.

	\param const std::string& _uniform
	The name of the uniform.
	*******************************************************************************/
	void RegisterUniform(const std::string& _uniform);

	/*!*****************************************************************************
	\brief
	Register uniforms and their location.

	\param const std::vector<std::string>& _strings
	The names of the uniforms.
	*******************************************************************************/
	void RegisterUniforms(const std::vector<std::string>& _strings);

	/*!*****************************************************************************
	\brief
	Inserts a uniform of type integer array into the shader program.

	\param const std::string& _uniform
	Name of the uniform.

	\param size_t _count
	Number of elements in the array.
	
	\param int* _data
	Pointer to the array.
	*******************************************************************************/
	void InsertUniform1iv(const std::string& _uniform, size_t _count, int* _data);

	/*!*****************************************************************************
	\brief
	Returns the ID of the shader program.

	\return
	Returns the ID of the shader program.
	*******************************************************************************/
	GLuint GetID() { return mProgramID; }

	/*!*****************************************************************************
	\brief
	Returns true if programs are linked, false otherwise.

	\return
	Returns true if programs are linked, false otherwise.
	*******************************************************************************/
	bool IsLinked() { return mLinked; }

private:
	bool mLinked;
	GLuint mProgramID;
	std::pair<std::string, std::string> mPaths;
	std::unordered_map<std::string, GLint> mUniforms;
};

