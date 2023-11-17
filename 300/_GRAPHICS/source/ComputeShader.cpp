#include "ComputeShader.hpp"

void GFX::ComputeShader::CreateShader(const char* shaderCode)
{

	// Compiling vertex shader code
	GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);
	if (!CompileErrors(shader, "COMPUTE"))		// Check for compiling errors
	{
		return;
	}

	// Create Shader Program
	mShaderProgram = glCreateProgram();

	// Attach compile vertex shader to shader program
	glAttachShader(mShaderProgram, shader);

	// Link Program
	glLinkProgram(mShaderProgram);

	if (!CompileErrors(mShaderProgram, "PROGRAM"))
	{
		return;
	}

	// Delete shader that has already been attached to program
	glDeleteShader(shader);
}

void GFX::ComputeShader::CreateShaderFromFile(const char* filePath)
{
	std::string computeCode{};

	// Get vertex code from file
	std::ifstream computeIfs(filePath, std::ios::binary);
	if (computeIfs)
	{
		// Get file size in bytes
		computeIfs.seekg(0, std::ios::end);
		computeCode.resize(computeIfs.tellg());

		// Rewind to the start of the file
		computeIfs.seekg(0, std::ios::beg);

		// Reading of whole file into string
		computeIfs.read(&computeCode[0], static_cast<std::streamsize>(computeCode.size()));
		computeIfs.close();

	}
	else
	{
		throw std::runtime_error("Unable to open Compute File!");
	}

	CreateShader(computeCode.c_str());
}

void GFX::ComputeShader::DestroyShader() const
{
	glDeleteProgram(mShaderProgram);
}

int32_t GFX::ComputeShader::GetUniformLocation(const char* uniformName) const
{
	return glGetUniformLocation(mShaderProgram, uniformName);
}

bool GFX::ComputeShader::CompileErrors(unsigned int shader, const char* shaderType) const
{
	GLint hasCompiled{};
	if (strcmp(shaderType, "PROGRAM") != 0)
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			std::cout << "SHADER COMPILATION_ERROR for: " << shaderType << '\n' << std::endl;
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
			std::cout << "SHADER_LINKING_ERROR for: " << shaderType << '\n' << std::endl;
			char infoLog[1024];
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << infoLog << "\n";
			return false;
		}
	}
	return true;
}
