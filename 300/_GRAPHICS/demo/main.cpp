
#include "DebugRenderer.hpp"
#include "Window.hpp"
#include "DemoScene.hpp"

int main()
{
    GFX::Window::InitializeSystem();
    GFX::Window window({ 1440, 810 });
    GFX::DebugRenderer renderer;
    GFX::DemoScene scene(window, &renderer);

    { // TODO: Main loop
        scene.Initialize();
        scene.Draw();
        scene.Exit();
    }
    GFX::Window::DestroySystem();
    return 0;
}