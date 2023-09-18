/**-------------------------------------------------------------------------------------/
 * @file
 *  Camera.hpp
 * @author
 *  Lee Fu Sheng Roy, 670000622, f.lee@digipen.edu
 * @date
 *  2023/05/21
 * @brief
 *  Class definition of Camera.
 * @copyright
 *  Copyright (C) 2023 DigiPen Institute of Technology.
 *-------------------------------------------------------------------------------------*/

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Math.hpp"

namespace GFX 
{
    class Camera
    {
    public:
        // -- Getter --
        vec3 position();
        vec3 target();
        vec3 direction();
        vec2 cursorPosition();
        mat4 viewProj();
        
        // -- Setter --
        void SetPosition(vec3 newPosition);
        void SetTarget(vec3 newTarget);
        void SetSize(ivec2 size);
        void SetCursorPosition(vec2 newPosition);
        void SetProjection(float fovDegree, ivec2 size, float nearZ, float farZ);

        void Update();

    private:
        mat4 mProjection;
        mat4 mView;
        mat4 mViewProjection;
        vec3 mPosition;
        vec3 mTarget;
        vec2 mCursorPos;
        ivec2 mSize;
        float mAspectRatio;
        float mNear;
        float mFar;
        float mFovDegree;
    };
}

#endif // CAMERA_HPP
