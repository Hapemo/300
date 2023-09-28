/*!*************************************************************************
****
\file			Helper.h
\author			Richmond
\par DP email:	r.choo@digipen.edu
\date			28/9/23
\brief
	The helper class that contain functions to help in the loading of
	assimp data

****************************************************************************
***/

#pragma once
#ifndef  _HELPER_H
#define  _HELPER_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <assimp/quaternion.h>
#include <assimp/vector3.h>
#include <assimp/matrix4x4.h>

namespace AssimpHelper
{

/***************************************************************************/
/*!
\brief
	this function is essential because asismp and glm uses different format
	to represent matrix
*/
/**************************************************************************/
	glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from);

/***************************************************************************/
/*!
\brief
	get the glm version of the assimp vec3
*/
/**************************************************************************/
	glm::vec3 GetGLMVec(const aiVector3D& vec);

/***************************************************************************/
/*!
\brief
	get the glm::quat versino of the assimp quaternion
*/
/**************************************************************************/
	glm::quat GetGLMQuat(const aiQuaternion& pOrientation);
}

#endif // ! _HELPER_H
