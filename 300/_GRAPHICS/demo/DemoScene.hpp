/**-------------------------------------------------------------------------------------/
 * @file
 *  DemoScene.hpp
 * @author
 *  Lee Fu Sheng Roy, 670000622, f.lee@digipen.edu
 * @date
 *  2023/05/21
 * @brief
 *  Class definition of DemoScene. DemoScene class showcases the different shapes that
    can be rendered
 * @copyright
 *  Copyright (C) 2023 DigiPen Institute of Technology.
 *-------------------------------------------------------------------------------------*/

#ifndef DEMOSCENE_HPP
#define DEMOSCENE_HPP

#include <memory>
#include "Camera.hpp"
#include "Window.hpp"
#include "DebugRenderer.hpp"
#include "Shapes.hpp"
#include "ImGui.hpp"
#include "Texture.hpp"

#include <geom.h>
#include <Animator.hpp>


namespace GFX {

    class DemoScene
    {
    public:
        DemoScene(Window const& window, DebugRenderer* renderer);

        // Initializes states and camera
        void Initialize();

        // Updates user inputs and camera's position and target
        void Update();

        // Renders onto screen with mRenderer
        void Draw();

        // Cleans up the DemoScene for any memory allocated
        void Exit();

    private:
        // -- Graphics --
        Window mWindow;
        DebugRenderer* mRenderer;
        Camera mCamera;
        Shader mModelShader;

        // -- Stats --
        float mDt = 0.0f;
        float mLastTime = 0.0f;
        float mCamSpeed = 50.f;

        // -- Flags --

        // -- Helper --
        
        // -- Textures --
        Texture mTexture;

        // -- Animator --
        Animator mObjectAnimator;       // this ideally should be within an entity. for now, its here
    };
}

#endif // DEMOSCENE_HPP
