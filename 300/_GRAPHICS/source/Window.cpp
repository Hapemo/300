/**-------------------------------------------------------------------------------------/
 * @file
 *  Window.cpp
 * @author
 *  Lee Fu Sheng Roy, 2101440, f.lee@digipen.edu
 * @date
 *  2023/05/21
 * @brief
 *  Class implementation of Window. Initializes GLFW and GLEW
 * @copyright
 *  Copyright (C) 2023 DigiPen Institute of Technology.
 *-------------------------------------------------------------------------------------*/

#include "Common.hpp"
#include "Window.hpp"


/**---------------------------------------------------------------------------/
 * @briefhhhhh
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
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    mWindow = glfwCreateWindow(mSize.x, mSize.y, "Graphics", nullptr, nullptr);

    if (mWindow == nullptr)
    {
        throw std::runtime_error("Failed to create a window!");
    }
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwMakeContextCurrent(mWindow);
    glfwSwapInterval(0);

    if (glewInit() != GLEW_OK)
    {
        throw std::runtime_error("Failed to initialize GLEW!");
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, nullptr);

    // Get the handle of the user's primary monitor
    mMonitor = glfwGetPrimaryMonitor();

    // Set the framebuffer resize callback functions
    glfwSetFramebufferSizeCallback(mWindow, ResizeCallback);
}

/**---------------------------------------------------------------------------/
 * @brief
 *  Destructor for the Window class. Destroys the window instance
 * @return
 *  none
*---------------------------------------------------------------------------*/
GFX::Window::~Window()
{
    //glfwDestroyWindow(mWindow);
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
    glfwGetFramebufferSize(mWindow, &mSize.x, &mSize.y);
    glfwSwapBuffers(mWindow);
}

void GFX::Window::SetFullScreenFlag(bool flag)
{
    if (mFullscreen == flag)    // Same flag, no change required
        return;

    mFullscreen = flag;

    if (flag)
    {
        glfwSetWindowMonitor(mWindow, mMonitor, 0, 0, mSize.x, mSize.y, GLFW_DONT_CARE);
        glfwGetFramebufferSize(mWindow, &mSize.x, &mSize.y);    // Update size of window
    }
    else
    {
        // Windowed mode
        glfwSetWindowMonitor(mWindow, NULL, 50, 50, mSize.x, mSize.y, GLFW_DONT_CARE);
        glfwGetFramebufferSize(mWindow, &mSize.x, &mSize.y);    // Update size of window
    }
}

vec2 GFX::Window::WindowToWorld(vec2 const& screenCoordinates)
{
    vec2 worldPos = screenCoordinates;
    worldPos.x -= mSize.x * 0.5f;
    worldPos.y *= -1;
    worldPos.y += mSize.y * 0.5f;;

    return worldPos;
}

int GFX::Window::ShouldClose()
{
    return glfwWindowShouldClose(mWindow);
}

void GFX::Window::CloseWindow()
{
    glfwSetWindowShouldClose(mWindow, GL_TRUE);
}

void GFX::Window::PollEvents()
{
    glfwPollEvents();
}

bool GFX::Window::IsKeyPressed(int key)
{
    return static_cast<bool>(glfwGetKey(mWindow, key));
}

bool GFX::Window::IsMousePressed(int key)
{
    return static_cast<bool>(glfwGetMouseButton(mWindow, key));
}

void GFX::Window::GetCursorPos(double* x, double* y)
{
    glfwGetCursorPos(mWindow, x, y);
}

void GFX::Window::SetWindowTitle(const char* title)
{
    glfwSetWindowTitle(mWindow, title);
}

void ResizeCallback(GLFWwindow* winPtr, int width, int height)
{
    glViewport(0, 0, width, height);

    (void)winPtr;
}