/*!*****************************************************************************
 \file OpenGl.cpp
 \author Lee Fu Sheng Roy
 \par DP email: f.lee@digipen.edu
 \par Group: Pepe Production
 \date 28-09-2023
 \brief
Includes graphics API libaries and has generic graphics utilities
*******************************************************************************/

#ifndef OPENGL_HPP
#define OPENGL_HPP

#include "Common.hpp"

/*!*****************************************************************************
Debugging callback function to be invoked whenever OpenGL encounters an error
*******************************************************************************/
namespace GFX
{
    void GLAPIENTRY MessageCallback(GLenum source,
                                    GLenum type,
                                    GLuint id,
                                    GLenum severity,
                                    GLsizei length,
                                    const char* message,
                                    const void* userParam);
}

#endif // OPENGL_HPP
