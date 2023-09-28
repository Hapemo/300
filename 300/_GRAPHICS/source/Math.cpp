/**-------------------------------------------------------------------------------------/
 * @file
 *  Math.cpp
 * @author
 *  Lee Fu Sheng Roy, 670000622, f.lee@digipen.edu
 * @date
 *  2023/05/21
 * @brief
 *  Class definition of Camera.
 * @copyright
 *  Copyright (C) 2023 DigiPen Institute of Technology.
 *-------------------------------------------------------------------------------------*/

#include "Math.hpp"

float GFX::Math::atan2(float x, float y)
{
	float result = std::atan2(y, x);

	if (x < 0 && y < 0)
	{
		result += glm::pi<float>() * 2.f;
	}
	return result;
}

vec2 GFX::Math::AngleToVec2(float angle)
{
	vec2 v{};
	v.x = glm::cos(angle);
	v.y = glm::sin(angle);

	return v;
}
