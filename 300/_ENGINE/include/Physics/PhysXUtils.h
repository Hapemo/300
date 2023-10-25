#pragma once
#include "PhysX.h"
#include "glm/glm.hpp"

using namespace physx;

/*!*****************************************************************************
Helper functions to convert glm to physx and vice versa.
*******************************************************************************/
PxVec3T<float> Convert(const glm::vec3& vec);
PxVec4T<float> Convert(const glm::vec4& vec);
PxQuatT<float> Convert(const glm::quat& quat);
glm::vec3 Convert(const PxVec3T<float>& vec);
glm::quat Convert(const PxQuatT<float>& vec);