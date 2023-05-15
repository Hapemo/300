/*!*****************************************************************************
\file GLShader.h
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 20-09-2022
\brief
This file contains function definitions to encapsulate a OpenGL shader object.
*******************************************************************************/
#include "Graphics/GLShader.h"

/*!*****************************************************************************
\brief
Constructor for GLShader class.

\param _vertexShader
File path to the vertex shader source code.

\param _fragmentShader
File path to the fragment shader source code.
*******************************************************************************/
GLShader::GLShader(const std::string& _vertexShader, const std::string& _fragmentShader)
{
	mProgramID = glCreateProgram();

	mLinked = false;
	mPaths.first = "../" + _vertexShader;
	mPaths.second = "../" + _fragmentShader;
}

/*!*****************************************************************************
\brief
Destructor for GLShader class.
*******************************************************************************/
GLShader::~GLShader()
{
	if (mProgramID > 0) 
		glDeleteProgram(mProgramID);
}

/*!*****************************************************************************
\brief
Compile the vertex shader's code.
*******************************************************************************/
void GLShader::CompileVertexShader()
{
	//opens the shader file
	std::ifstream vertexShader(mPaths.first, std::ifstream::in);

	//error checking
	if (!vertexShader) {
		THROW(Logger::E_EXCEPTION::RUNTIME_ERR, "File for vertex shader not found!");
	}

	//reading file into stringstream
	std::stringstream ss;
	ss << vertexShader.rdbuf();
	vertexShader.close();

	std::string str = ss.str();

	//creating vertex shader
	GLuint vertexHandle = glCreateShader(GL_VERTEX_SHADER);

	GLchar const* vertexCode[] = { str.c_str() };

	//inserting source code and compiling
	glShaderSource(vertexHandle, 1, vertexCode, NULL);
	glCompileShader(vertexHandle);

	GLint result;
	glGetShaderiv(vertexHandle, GL_COMPILE_STATUS, &result);

	//error checking
	if (result == GL_FALSE)
		throw std::string("Vertex shader compilation failed.");

	//attach shader to shader program
	glAttachShader(mProgramID, vertexHandle);
}

/*!*****************************************************************************
\brief
Compile the fragment shader's code.
*******************************************************************************/
void GLShader::CompileFragmentShader()
{
	//opens the shader file
	std::ifstream fragmentShader(mPaths.second, std::ifstream::in);

	//error checking
	if (!fragmentShader)
		throw std::string("File for fragment shader not found!");

	//reading file into stringstream
	std::stringstream ss;
	ss << fragmentShader.rdbuf();
	fragmentShader.close();

	std::string str = ss.str();

	//creating the shader program
	GLuint fragmentHandle = glCreateShader(GL_FRAGMENT_SHADER);
	GLchar const* fragmentCode[] = { str.c_str() };

	//inserting source code and compiling
	glShaderSource(fragmentHandle, 1, fragmentCode, NULL);
	glCompileShader(fragmentHandle);

	GLint result;
	glGetShaderiv(fragmentHandle, GL_COMPILE_STATUS, &result);

	//error checking
	if (result == GL_FALSE)
		throw std::string("Fragment shader compilation failed.");

	//attach shader to shader program
	glAttachShader(mProgramID, fragmentHandle);
}

/*!*****************************************************************************
\brief
Compile both vertex and fragment shader's code.
*******************************************************************************/
void GLShader::CompileShaders()
{
	CompileVertexShader(), CompileFragmentShader();
}

/*!*****************************************************************************
\brief
Links vertex and fragment shaders.
*******************************************************************************/
void GLShader::LinkShaders()
{
	if (mLinked)
		return;

	//links the 2 shaders
	glLinkProgram(mProgramID);

	GLint result;
	glGetProgramiv(mProgramID, GL_LINK_STATUS, &result);

	//error checking
	if (result == GL_FALSE)
		throw std::string("Linking of shaders failed.");

	mLinked = true;
}

/*!*****************************************************************************
\brief
Compile and link both vertex and fragment shader's code.
*******************************************************************************/
void GLShader::CompileLinkShaders()
{
	CompileShaders(), LinkShaders();
}

/*!*****************************************************************************
\brief
Checks if vertex and fragment shaders are compiled and linked correctly, will
throw an exception otherwise.
*******************************************************************************/
void GLShader::Validate()
{
	//checks the state of the program
	glValidateProgram(mProgramID);

	GLint result;
	glGetProgramiv(mProgramID, GL_VALIDATE_STATUS, &result);

	//error checking
	if (result == GL_FALSE)
		throw std::string("Validation of shaders failed");
}

/*!*****************************************************************************
\brief
Binds the shader program to the OpenGL context
*******************************************************************************/
void GLShader::Bind()
{
	glUseProgram(mProgramID);
}

/*!*****************************************************************************
\brief
Unbinds the shader program to the OpenGL context
*******************************************************************************/
void GLShader::Unbind()
{
	glUseProgram(0);
}

/*!*****************************************************************************
\brief
Register a uniform and its location.

\param const std::string& _uniform
The name of the uniform.
*******************************************************************************/
void GLShader::RegisterUniform(const std::string& _uniform)
{
	mUniforms.insert(std::pair<std::string, GLint>(_uniform, 
		glGetUniformLocation(GetID(), _uniform.c_str())));
	
	if (mUniforms[_uniform] < 0)
		throw std::string("Uniform not found!");
}

/*!*****************************************************************************
\brief
Register uniforms and their location.

\param const std::vector<std::string>& _strings
The names of the uniforms.
*******************************************************************************/
void GLShader::RegisterUniforms(const std::vector<std::string>& _strings)
{
	for (const std::string& str : _strings)
		RegisterUniform(str);
}

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
void GLShader::InsertUniform1iv(const std::string& _uniform, size_t _count, int* _data)
{
	glUniform1iv(mUniforms[_uniform], static_cast<GLsizei>(_count), _data);
}