
#include "DebugRenderer.hpp"
#include "Window.hpp"
#include "DemoScene.hpp"

int main()
{
    CS380::Window::InitializeSystem();
    CS380::Window window({ 1440, 810 });
    CS380::DebugRenderer renderer;
    CS380::DemoScene scene(window, &renderer);

    { // TODO: Main loop
        scene.Initialize();
        scene.Draw();
        scene.Exit();
    }
    CS380::Window::DestroySystem();
    return 0;
}