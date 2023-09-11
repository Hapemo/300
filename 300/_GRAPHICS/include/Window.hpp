/**-------------------------------------------------------------------------------------/
 * @file
 *  Window.hpp
 * @author
 *  Lee Fu Sheng Roy, 2101440, f.lee@digipen.edu
 * @date
 *  2023/05/21
 * @brief
 *  Class definition of Window. Initializes GLFW and GLEW
 * @copyright
 *  Copyright (C) 2023 DigiPen Institute of Technology.
 *-------------------------------------------------------------------------------------*/
#pragma once
#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "OpenGl.hpp"
#include "Math.hpp"

namespace GFX {
    class Window
    {
    public:
        Window() : mSize{}, mWindow{ nullptr } {};
        Window(ivec2 windowSize);
        Window(Window const&) = default;
        Window& operator=(Window const&) = default;
        ~Window();

        static void InitializeSystem();
        static void DestroySystem();
        void Update();

        // -- Getter --
        double GetTime()        { return glfwGetTime(); }   // Retrieves the current run time of the window
        GLFWwindow* GetHandle() { return mWindow; }         // Retrieves the GLFW handle of the window instance
        ivec2 size()            { return mSize; }           // Retrieves the width and height of window

        void SetWindowTitle(const char* title);

        // -- Input --
        void PollEvents();
        bool IsKeyPressed(int key);         // Keyboard
        bool IsMousePressed(int key);       // Mouse
        void GetCursorPos(double* x, double* y);

        // -- NOT TESTED YET --
        vec2 WindowToWorld(vec2 const& screenCoordinates);   // maps glfw coordinates to world coordinates
        int ShouldClose();
        void CloseWindow();

    private:
        ivec2 mSize;
        GLFWwindow* mWindow;
    };
}



#endif // WINDOW_HPP
