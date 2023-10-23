#include "Physics/PhysXUtils.h"
#include "ECS/ECS_Components.h"

physx::PxVec3T<float> Convert(const glm::vec3& vec)
{
	return physx::PxVec3T<float>(vec.x, vec.y, vec.z);
}

physx::PxVec4T<float> Convert(const glm::vec4& vec)
{
	return physx::PxVec4T<float>(vec.x, vec.y, vec.z, vec.w);
}

glm::vec3 Convert(const physx::PxVec3T<float>& vec)
{
	return { vec.x, vec.y, vec.z };
}

glm::quat Convert(const physx::PxQuatT<float>& vec)
{
	return glm::quat{ vec.x, vec.y, vec.z, vec.w };
}