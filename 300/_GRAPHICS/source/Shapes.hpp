#ifndef SHAPES_HPP
#include "Common.hpp"

namespace GFX
{
	struct Aabb
	{
		Aabb() : mCenter{ 0, 0, 0 }, mSize{ 0, 0, 0 }  {}
		Aabb(vec3 const& center, vec3 const& size) : mCenter{ center }, mSize{ size } {}

		vec3 mCenter;
		vec3 mSize;
	};
}

#endif // !SHAPES_HPP

