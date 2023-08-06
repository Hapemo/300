#include "Math.hpp"

float CS380::Math::atan2(float x, float y)
{
	float result = std::atan2(y, x);

	if (x < 0 && y < 0)
	{
		result += glm::pi<float>() * 2.f;
	}
	return result;
}

vec2 CS380::Math::AngleToVec2(float angle)
{
	vec2 v{};
	v.x = glm::cos(angle);
	v.y = glm::sin(angle);

	return v;
}
