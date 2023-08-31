/**-------------------------------------------------------------------------------------/
 * @file
 *  Window.hpp
 * @author
 *  Lee Fu Sheng Roy, 670000622, f.lee@digipen.edu
 * @date
 *  2023/05/21
 * @brief
 *  Class definition of Window. Initializes GLFW and GLAD
 * @copyright
 *  Copyright (C) 2023 DigiPen Institute of Technology.
 *-------------------------------------------------------------------------------------*/

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

        // Retrieves the GLFW handle of the window instance
        GLFWwindow* GetHandle() { return mWindow; }

        void SetWindowTitle(const char* title);

        vec2 WindowToWorld(vec2 const& screenCoordinates);   // maps glfw coordinates to world coordinates

        // -- Getter --
        ivec2 size();

    private:
        ivec2 mSize;
        GLFWwindow* mWindow;
    };
}

#endif // WINDOW_HPP
