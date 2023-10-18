/*!*****************************************************************************
\file Camera.hpp
\author Lee Fu Sheng Roy
\par DP email: f.lee@digipen.edu
\par Group: Pepe Production
\date 28-09-2023
\brief
Camera class implementation. Consists of the functions to get/set position,
target, direction, cursor position, view projection and projection of the camera
instance
*******************************************************************************/

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Math.hpp"

namespace GFX 
{
    namespace CameraConstants {
        const float maxFOV = 45.f;
        const float minFOV = 1.f;
    }

    class Camera
    {
    public:

/*!*****************************************************************************
Default constructor of the Camera class. Sets the default parameters of the
camera object
*******************************************************************************/
        Camera();

/*!*****************************************************************************
Gets the position of the camera
*******************************************************************************/
        vec3 position();
        
/*!*****************************************************************************
Gets the target of the camera
*******************************************************************************/
        vec3 target();
        
/*!*****************************************************************************
Gets the view direction of the camera
*******************************************************************************/
        vec3 direction();
        
/*!*****************************************************************************
Gets the cursor position of the camera
*******************************************************************************/
        vec2 cursorPosition();
        
/*!*****************************************************************************
Gets the view projection matrix of the camera
*******************************************************************************/
        mat4 viewProj();
        
/*!*****************************************************************************
Gets the projection matrix of the camera
*******************************************************************************/
        mat4 Proj();

/*!*****************************************************************************
Sets the position of the camera
*******************************************************************************/
        void SetPosition(vec3 newPosition);
        
/*!*****************************************************************************
Sets the target of the camera
*******************************************************************************/
        void SetTarget(vec3 newTarget);
        
/*!*****************************************************************************
Sets the size of the camera
*******************************************************************************/
        void SetSize(ivec2 size);
        
/*!*****************************************************************************
Sets the cursor position of the camera
*******************************************************************************/
        void SetCursorPosition(vec2 newPosition);
        
/*!*****************************************************************************
Sets the projection matrix of the camera
*******************************************************************************/
        void SetProjection(float fovDegree, ivec2 size, float nearZ, float farZ);

/*!*****************************************************************************
Updates projection and view projection matrix of the camera
*******************************************************************************/
        void Update();

        mat4 mProjection;
        mat4 mView;


        mat4    mViewProjection;
        vec3    mPosition; //save
        vec3    mTarget;   //save
        vec2    mCursorPos;
        ivec2   mSize;    //save
        float   mAspectRatio;
        float   mNear;    //save
        float   mFar;     //save
        float   mFovDegree;
        float	mYaw, mPitch;
    };
}

#endif // CAMERA_HPP
