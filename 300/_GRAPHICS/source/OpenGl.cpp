/*!*****************************************************************************
 \file OpenGl.cpp
 \author Lee Fu Sheng Roy
 \par DP email: f.lee@digipen.edu
 \par Group: Pepe Production
 \date 28-09-2023
 \brief
Includes graphics API libaries and has generic graphics utilities
*******************************************************************************/

#define _GLDEBUG 0

#include "OpenGl.hpp"

void GLAPIENTRY GFX::MessageCallback(GLenum source,
                                    GLenum type,
                                    GLuint id,
                                    GLenum severity,
                                    GLsizei length,
                                    const char* message,
                                    const void* userParam)
{
#if _GLDEBUG
    std::cout << "GL Callback: ";
    std::cout << " *ERROR* ";
    std::cout << "Source: " << source << " | ID: " << id;
    std::cout << " | Type: " << type << " | Severity: " << severity;
    std::cout << " | Message: " << message << "\n\n";
#endif

    (void)length;
    (void)userParam;
}