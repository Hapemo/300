/**-------------------------------------------------------------------------------------/
 * @file
 *  OpenGl.hpp
 * @author
 *  Lee Fu Sheng Roy, 670000622, f.lee@digipen.edu
 * @date
 *  2023/05/21
 * @brief
 *  Includes graphics API libaries and has generic graphics utilities
 * @copyright
 *  Copyright (C) 2023 DigiPen Institute of Technology.
 *-------------------------------------------------------------------------------------*/
#ifndef OPENGL_HPP
#define OPENGL_HPP

#include "Common.hpp"

namespace CS380
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
