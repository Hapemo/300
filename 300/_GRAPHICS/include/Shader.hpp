/*!*****************************************************************************
\file Shader.hpp
\author Lee Fu Sheng Roy
\par DP email: f.lee@digipen.edu
\par Group: Pepe Production
\date 28-09-2023
\brief
Shader class implementation for loading, creating, compiling and linking shader
*******************************************************************************/

#ifndef SHADER_HPP
#define SHADER_HPP

#include "Common.hpp"

namespace GFX 
{
    class Shader
    {
    public:
/*!*****************************************************************************
Creates the shader program by compiling the vertex and fragment shader code. 
Compiled shader will be attached and link the shader program
*******************************************************************************/
        void CreateShader(const char* shaderCode, const char* fragmentShaderCode);

/*!*****************************************************************************
Creates the shader program by compiling the vertex and fragment shader code.
Compiled shader will be attached and link the shader program
*******************************************************************************/
        void CreateShaderFromFiles(const char* vertexFile, const char* fragmentFile);

/*!*****************************************************************************
Deletes the shader program from the GPU's memory
*******************************************************************************/
        void DestroyShader() const;

/*!*****************************************************************************
Activates the shader program
*******************************************************************************/
        void Activate() const;

/*!*****************************************************************************
Deactivates the shader program
*******************************************************************************/
        static void Deactivate();

/*!*****************************************************************************
Get the handle of the shader program
*******************************************************************************/
        uint32_t GetHandle() const { return mShaderProgram; }

/*!*****************************************************************************
Get the uniform location of the view projection matrix of the activated shader 
program
*******************************************************************************/
        int32_t GetUniformVP() const { return mUniformVp; }

/*!*****************************************************************************
Gets the uniform location of uniformName from the activated shader program
*******************************************************************************/
        int32_t GetUniformLocation(const char* uniformName) const;

    private:
        int32_t  mUniformVp = -1;
        int32_t  mUniformM2W = -1;
        int32_t  mUniformColor = -1;
        uint32_t mShaderProgram{};

/*!*****************************************************************************
Compiles the shader program and checks for compilation or linking errors
*******************************************************************************/
        bool CompileErrors(unsigned int shader, const char* shaderType) const;
    };
}
#endif // SHADER_HPP