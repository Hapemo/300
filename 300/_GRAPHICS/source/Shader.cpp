/*!*****************************************************************************
\file Shader.cpp
\author Lee Fu Sheng Roy
\par DP email: f.lee@digipen.edu
\par Group: Pepe Production
\date 28-09-2023
\brief
Shader class implementation for loading, creating, compiling and linking shader
*******************************************************************************/

#include "Shader.hpp"

void GFX::Shader::CreateShader(const char* vertexShaderCode, const char* fragmentShaderCode, const char* vertFile, const char* fragFile)
{
	// Create Shader Program
	mShaderProgram = glCreateProgram();

	// Compiling vertex shader code
	GLuint shader = glCreateShader(GL_VERTEX_SHADER);
	int32_t shaderCodeLength = (int32_t)strlen(vertexShaderCode);
	glShaderSource(shader, 1, &vertexShaderCode, &shaderCodeLength);
	glCompileShader(shader);
	if (!CompileErrors(shader, "VERTEX", vertFile, fragFile))		// Check for compiling errors
	{
		return;
	}

	// Attach compile vertex shader to shader program
	glAttachShader(mShaderProgram, shader);

	// Compiling fragment shader code
	shader = glCreateShader(GL_FRAGMENT_SHADER);
	shaderCodeLength = (int32_t)strlen(fragmentShaderCode);
	glShaderSource(shader, 1, &fragmentShaderCode, &shaderCodeLength);
	glCompileShader(shader);
	if (!CompileErrors(shader, "FRAGMENT", vertFile, fragFile))		// Check for compiling errors
	{
		return;
	}

	// Attach compile vertex shader to shader program
	glAttachShader(mShaderProgram, shader);

	// Link Program
	glLinkProgram(mShaderProgram);
	if (!CompileErrors(mShaderProgram, "PROGRAM", vertFile, fragFile))
	{
		return;
	}

	// Set up uniform objects
	mUniformVp = glGetUniformLocation(mShaderProgram, "uMatrixVP");

	// Delete shader that has already been attached to program
	glDeleteShader(shader);
}

void GFX::Shader::CreateShaderFromFiles(const char* vertexFile, const char* fragmentFile)
{
	std::string vertexCode{};
	std::string fragmentCode{};

	// Get vertex code from file
	std::ifstream vertIfs(vertexFile, std::ios::binary);
	if (vertIfs)
	{
		// Get file size in bytes
		vertIfs.seekg(0, std::ios::end);
		vertexCode.resize(vertIfs.tellg());
	
		// Rewind to the start of the file
		vertIfs.seekg(0, std::ios::beg);
	
		// Reading of whole file into string
		vertIfs.read(&vertexCode[0], static_cast<std::streamsize>(vertexCode.size()));
		vertIfs.close();

	}
	else
	{
		throw std::runtime_error("Unable to open Vertex File!");
	}

	// Get fragment code from file
	std::ifstream fragIfs(fragmentFile, std::ios::binary);
	if (fragIfs)
	{
		// Get file size in bytes
		fragIfs.seekg(0, std::ios::end);
		fragmentCode.resize(fragIfs.tellg());

		// Rewind to the start of the file
		fragIfs.seekg(0, std::ios::beg);

		// Reading of whole file into string
		fragIfs.read(&fragmentCode[0], static_cast<std::streamsize>(fragmentCode.size()));
		fragIfs.close();
	}
	else
	{
		throw std::runtime_error("Unable to open Fragment File!");
	}

	CreateShader(vertexCode.c_str(), fragmentCode.c_str(), vertexFile, fragmentFile);
}

/**---------------------------------------------------------------------------/
 * @brief
 *  Deletes the shader program from the GPU's memory
 * @return
 *  none
*---------------------------------------------------------------------------*/
void GFX::Shader::DestroyShader() const
{
	glDeleteProgram(mShaderProgram);
}

/**---------------------------------------------------------------------------/
 * @brief
 *  Links the shader program to be used in the render pass
 * @return
 *  none
*---------------------------------------------------------------------------*/
void GFX::Shader::Activate() const
{
	glUseProgram(mShaderProgram);
}

/**---------------------------------------------------------------------------/
 * @brief
 *  Unlinks the shader program
 * @return
 *  none
*---------------------------------------------------------------------------*/
void GFX::Shader::Deactivate()
{
	glUseProgram(0);
}

/**---------------------------------------------------------------------------/
 * @brief
 *  Unlinks the shader program
 * @param shader
 *	shader ID
 * @param shaderType
 *	Type of shader to be checked for errors.
 * @return
 *  none
*---------------------------------------------------------------------------*/
bool GFX::Shader::CompileErrors(unsigned int shader, const char* shaderType, const char* vertFile, const char* fragFile) const
{
	GLint hasCompiled{};
	if (strcmp(shaderType, "PROGRAM") != 0)
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			std::cout << "SHADER COMPILATION_ERROR for: ";
			if (!strcmp(shaderType, "VERTEX"))
				std::cout << vertFile << " | ";
			if (!strcmp(shaderType, "FRAGMENT"))
				std::cout << fragFile << " | ";

			std::cout << shaderType << '\n' << std::endl;
			char infoLog[1024];
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << infoLog << "\n";
			return false;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			std::cout << "SHADER_LINKING_ERROR for: ";
			std::cout << vertFile << " | ";
			std::cout << fragFile << " | ";
			std::cout << shaderType << '\n' << std::endl;
			char infoLog[1024];
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << infoLog << "\n";
			return false;
		}
	}
	return true;
}

int32_t GFX::Shader::GetUniformLocation(const char* uniformName) const
{
	return glGetUniformLocation(mShaderProgram, uniformName);
}