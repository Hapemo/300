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

namespace CS380 {
    class Camera
    {
    public:
        // -- Getter --
        vec3 position();
        vec3 direction();
        vec2 cursorPosition();
        mat4 viewProj();
        
        // -- Setter --
        void SetPosition(vec3 newPosition);
        void SetTarget(vec3 newTarget);
        void SetCursorPosition(vec2 newPosition);
        void SetProjection(float left, float right, float bottom, float top, float near, float far);

        void Update();

    private:
        vec3 mPosition;
        vec3 mTarget;
        vec2 mCursorPos;
        mat4 mProjection;
        mat4 mView;
        mat4 mViewProjection;
    };
}

#endif // CAMERA_HPP
