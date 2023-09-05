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
	glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from);
	glm::vec3 GetGLMVec(const aiVector3D& vec);
	glm::quat GetGLMQuat(const aiQuaternion& pOrientation);
}

#endif // ! _HELPER_H
