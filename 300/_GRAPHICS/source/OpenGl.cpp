/**-------------------------------------------------------------------------------------/
 * @file
 *  OpenGl.cpp
 * @author
 *  Lee Fu Sheng Roy, 670000622, f.lee@digipen.edu
 * @date
 *  2023/05/21
 * @brief
 *  Includes graphics API libaries and has generic graphics utilities
 * @copyright
 *  Copyright (C) 2023 DigiPen Institute of Technology.
 *-------------------------------------------------------------------------------------*/
#include "OpenGl.hpp"

/**---------------------------------------------------------------------------/
 * @brief
 *  Debugging callback function to be invoked whenever OpenGL encounters an 
 *  error
 * @param source
 *  Source of the error
 * @param type
 *  Type of error
 * @param id
 *  ID of the error
 * @param severity
 *  Severity of the error
 * @param length
 *  Length of message
 * @param message
 *  Message of the error
 * @param userParam
 *  Additional user parameters
 * @return
 *  None
*---------------------------------------------------------------------------*/
void GLAPIENTRY GFX::MessageCallback(GLenum source,
                                    GLenum type,
                                    GLuint id,
                                    GLenum severity,
                                    GLsizei length,
                                    const char* message,
                                    const void* userParam)
{
    std::cout << "GL Callback: ";
    std::cout << " *ERROR* ";
    std::cout << "Source: " << source << " | ID: " << id;
    std::cout << " | Type: " << type << " | Severity: " << severity;
    std::cout << " | Message: " << message << "\n";

    (void)length;
    (void)userParam;
}