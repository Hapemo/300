/**-------------------------------------------------------------------------------------/
 * @file
 *  Window.cpp
 * @author
 *  Lee Fu Sheng Roy, 670000622, f.lee@digipen.edu
 * @date
 *  2023/05/21
 * @brief
 *  Class implementation of Window. Initializes GLFW and GLAD
 * @copyright
 *  Copyright (C) 2023 DigiPen Institute of Technology.
 *-------------------------------------------------------------------------------------*/

#include "Common.hpp"
#include "Window.hpp"


/**---------------------------------------------------------------------------/
 * @brief
 *  Constructor for the Window class. Creates a window of specified size
 * @param windowSize
 *	width and height of the window
 * @param windowVisible
 *	Flag for the visibility of window
 * @return
 *  none
*---------------------------------------------------------------------------*/
GFX::Window::Window(ivec2 windowSize)
{
    mSize = windowSize;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    mWindow = glfwCreateWindow(mSize.x, mSize.y, "Graphics", nullptr, nullptr);
    if (mWindow == nullptr)
    {
        throw std::runtime_error("Failed to create a window!");
    }

    glfwMakeContextCurrent(mWindow);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        throw std::runtime_error("Failed to initialize GLEW!");
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, nullptr);
}

/**---------------------------------------------------------------------------/
 * @brief
 *  Destructor for the Window class. Destroys the window instance
 * @return
 *  none
*---------------------------------------------------------------------------*/
GFX::Window::~Window()
{
    glfwDestroyWindow(mWindow);
}

/**---------------------------------------------------------------------------/
 * @brief
 *  Initializes GLFW
 * @return
 *  none
*---------------------------------------------------------------------------*/
void GFX::Window::InitializeSystem()
{
    // Initialize OpenGL
    if (glfwInit() == 0)
    {
        throw std::runtime_error("Failed to initialize GLFW!");
    }
}

/**---------------------------------------------------------------------------/
 * @brief
 *  Terminates GLFW
 * @return
 *  none
*---------------------------------------------------------------------------*/
void GFX::Window::DestroySystem()
{
    glfwTerminate();
}

/**---------------------------------------------------------------------------/
 * @brief
 *  Swap the front buffer with the back buffer
 * @return
 *  none
*---------------------------------------------------------------------------*/
void GFX::Window::Update()
{
    glfwSwapBuffers(mWindow);
}

vec2 GFX::Window::WindowToWorld(vec2 const& screenCoordinates)
{
    vec2 worldPos = screenCoordinates;
    worldPos.x -= mSize.x * 0.5f;
    worldPos.y *= -1;
    worldPos.y += mSize.y * 0.5f;;

    return worldPos;
}

/**---------------------------------------------------------------------------/
 * @brief
 *  Gets the width and height of the window
 * @return
 *  Width and height of the window
*---------------------------------------------------------------------------*/
ivec2 GFX::Window::size()
{
	return mSize;
}
