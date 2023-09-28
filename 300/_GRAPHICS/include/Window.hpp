/*!*****************************************************************************
\file Window.hpp
\author Lee Fu Sheng Roy
\par DP email: f.lee@digipen.edu
\par Group: Pepe Production
\date 28-09-2023
\brief
Creates and Manages the Window of the application. Initializes GLFW and GLEW
*******************************************************************************/
#pragma once
#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "OpenGl.hpp"
#include "Math.hpp"

namespace GFX {
    class Window
    {
    public:
/*!*****************************************************************************
Default constructor of the Window class
*******************************************************************************/
        Window() : mSize{}, mWindow{ nullptr } {};

/*!*****************************************************************************
Single argument constructor of the Window class
*******************************************************************************/
        Window(ivec2 windowSize);

/*!*****************************************************************************
Copy constructor of the Window class
*******************************************************************************/
        Window(Window const&) = default;

/*!*****************************************************************************
Copy assignment operator of the Window class
*******************************************************************************/
        Window& operator=(Window const&) = default;

/*!*****************************************************************************
Destructor of the Window class
*******************************************************************************/
        ~Window();

/*!*****************************************************************************
Initializes the GLFW library
*******************************************************************************/
        static void InitializeSystem();

/*!*****************************************************************************
Terminates the GLFW system
*******************************************************************************/
        static void DestroySystem();

/*!*****************************************************************************
Updates the Window application by swapping the buffers
*******************************************************************************/
        void Update();

/*!*****************************************************************************
Flag to set the Window application to fullscreen
*******************************************************************************/
        void SetFullScreenFlag(bool flag);

        // -- Getter --  
/*!*****************************************************************************
Returns the GLFW time of the window
*******************************************************************************/
        double GetTime()            { return glfwGetTime(); }
        
/*!*****************************************************************************
Returns the GLFWwindow* of the Window Application
*******************************************************************************/
        GLFWwindow* GetHandle()     { return mWindow; }
        
/*!*****************************************************************************
Returns the size of the Window Application
*******************************************************************************/
        ivec2 size()                { return mSize; }
        
/*!*****************************************************************************
Returns the width of the Window Application
*******************************************************************************/
        int GetScreenWidth()        { return mSize.x; }
        
/*!*****************************************************************************
Returns the height of the Window Application
*******************************************************************************/
        int GetScreenHeight()       { return mSize.y; }
        
/*!*****************************************************************************
Returns the fullscreen flag of the Window Application
*******************************************************************************/
        bool GetFullScreenFlag()    { return mFullscreen; }
        
/*!*****************************************************************************
Returns if the window application is in focused or not
*******************************************************************************/
        bool GetWindowMinimised()   { return !glfwGetWindowAttrib(mWindow, GLFW_FOCUSED); }
        
/*!*****************************************************************************
Set the window title of the Window application
*******************************************************************************/
        void SetWindowTitle(const char* title);

        // -- Input --
/*!*****************************************************************************
Wrapper function for the glfwPollEvents function
*******************************************************************************/        
        void PollEvents();

/*!*****************************************************************************
Checks if a certain key is being pressed
*******************************************************************************/        
        bool IsKeyPressed(int key);

/*!*****************************************************************************
Checks if a certain mouse key is being pressed
*******************************************************************************/        
        bool IsMousePressed(int key);

/*!*****************************************************************************
Gets the position of the cursor
*******************************************************************************/        
        void GetCursorPos(double* x, double* y);

        // -- NOT TESTED YET --

/*!*****************************************************************************
Maps the screen coordinates in window space to world space
*******************************************************************************/
        vec2 WindowToWorld(vec2 const& screenCoordinates);   // maps glfw coordinates to world coordinates
        
/*!*****************************************************************************
Wrapper function for the glfwWindowShouldClose function
*******************************************************************************/ 
        int ShouldClose();
        
/*!*****************************************************************************
Sets the glfwSetWindowShouldClose function to true which forces the window to 
close
*******************************************************************************/ 
        void CloseWindow();

    private:
        ivec2 mSize;
        GLFWwindow* mWindow{};
        GLFWmonitor* mMonitor{};

        // -- Flags -- 
        bool mFullscreen{};
    };

}
/*!*****************************************************************************
Callback function that will be invoked whenever the size of the Window changes
*******************************************************************************/
void ResizeCallback(GLFWwindow* winPtr, int width, int height);
#endif // WINDOW_HPP
