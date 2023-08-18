/**-------------------------------------------------------------------------------------/
 * @file
 *  Shader.hpp
 * @author
 *  Lee Fu Sheng Roy, 670000622, f.lee@digipen.edu
 * @date
 *  2023/05/21
 * @brief
 *  Class definition of Shader. Contains the code to create shader program by compiling 
 *  and linking the shader program
 * @copyright
 *  Copyright (C) 2023 DigiPen Institute of Technology.
 *-------------------------------------------------------------------------------------*/

#ifndef SHADER_HPP
#define SHADER_HPP

#include "Common.hpp"

namespace GFX 
{
    class Shader
    {
    public:
        void CreateShader(const char* shaderCode, const char* fragmentShaderCode);
        void CreateShaderFromFiles(const char* vertexFile, const char* fragmentFile);
        void DestroyShader() const;
        void Activate() const;
        void Deactivate() const;

        // -- Getter --
        int32_t GetUniformVP() const { return mUniformVp; }
        int32_t GetUniformM2W() const { return mUniformM2W; }
        int32_t GetUniformColor() const { return mUniformColor; }
        int32_t GetUniformLocation(const char* uniformName) const;

    private:
        int32_t  mUniformVp = -1;
        int32_t  mUniformM2W = -1;
        int32_t  mUniformColor = -1;
        uint32_t mShaderProgram{};

        bool CompileErrors(unsigned int shader, const char* shaderType) const;
    };
}
#endif // SHADER_HPP