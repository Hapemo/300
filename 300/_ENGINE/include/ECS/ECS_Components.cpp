#include "ECS_Components.h"

mat4 Portal::GetClippedProjMatrix(Transform const& destTransform, float distance, mat4 const& destViewMatrix, mat4 const& projMatrix)
{
    // Get Plane normal of destination portal
    mat3 R = glm::toMat3(glm::quat(glm::radians(destTransform.mRotate)));
    vec3 planeNormal = R * vec3(0.f, 0.f, -1.f);

    // Get clip plane
    vec4 clipPlane = vec4(planeNormal, glm::dot(destTransform.mTranslate, planeNormal));

    // Calculate Reflection
    vec4 q = glm::inverse(projMatrix) * clipPlane;
    vec4 c = 2.f * q / glm::dot(q, clipPlane);

    mat4 obliqueMatrix = projMatrix;
    obliqueMatrix[2]    = c.x - projMatrix[3];
    obliqueMatrix[6]    = c.y - projMatrix[7];
    obliqueMatrix[10]   = c.z - projMatrix[11];
    obliqueMatrix[14]   = c.w - projMatrix[15];

    return obliqueMatrix;
}
