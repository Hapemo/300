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
        mat4 Proj();


        // -- Setter --
        void SetPosition(vec3 newPosition);
        void SetTarget(vec3 newTarget);
        void SetSize(ivec2 size);
        void SetCursorPosition(vec2 newPosition);
        void SetProjection(float fovDegree, ivec2 size, float nearZ, float farZ);

        void Update();

        float mFovDegree;
        mat4 mProjection;
        mat4 mView;


        mat4 mViewProjection;
        vec3 mPosition; //save
        vec3 mTarget;   //save
        vec2 mCursorPos;
        ivec2 mSize;    //save
        float mAspectRatio;
        float mNear;    //save
        float mFar;     //save
    };
}

#endif // CAMERA_HPP
