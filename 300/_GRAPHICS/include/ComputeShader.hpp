#ifndef COMPUTER_SHADER_HPP
#define COMPUTER_SHADER_HPP

#include "Common.hpp"

namespace GFX
{
	class ComputeShader
	{
	public:
		void CreateShader(const char* shaderCode);
		void CreateShaderFromFile(const char* filePath);
		void DestroyShader() const;

		void Activate() const { glUseProgram(mShaderProgram); }
		void Deactivate() const { glUseProgram(0); }

		uint32_t GetHandle() const { return mShaderProgram; }
		int32_t GetUniformLocation(const char* uniformName) const;


	private:
		uint32_t mShaderProgram{};

		bool CompileErrors(unsigned int shader, const char* shaderType) const;
	};
}

#endif // !COMPUTER_SHADER_HPP
