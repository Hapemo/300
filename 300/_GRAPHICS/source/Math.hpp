/**
 * @file Math.hpp
 * @author Eder Beldad (eder.b1@digipen.edu)
 * @brief Contains basic math includes
 * @date 2023-05-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef MATH_HPP
#define MATH_HPP

#define GLM_FORCE_INLINE
#define GLM_FORCE_NO_CTOR_INIT
#define GLM_FORCE_EXPLICIT_CTOR
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_XYZW_ONLY // Incompatible with colors in HSV

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/integer.hpp>
#include <glm/gtx/orthonormalize.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/random.hpp>

constexpr float cEpsilon = 1e-5f;

// Easy access
using glm::ivec2;
using glm::ivec3;
using glm::ivec4;
using glm::mat2;
using glm::mat3;
using glm::mat4;
using glm::quat;
using glm::vec2;
using glm::vec3;
using glm::vec4;

namespace GFX {
	namespace Math
	{
		float atan2(float x, float y);
		vec2 AngleToVec2(float angle);
	}
}

#endif // MATH_HPP
