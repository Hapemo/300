#pragma once

enum class MATERIAL : unsigned char
{
	RUBBER,
	WOOD,
	METAL,
	ICE,
	CONCRETE,
	GLASS,
	UNDEFINED // for serialization helper functions
};

enum class MOTION : unsigned char
{
	STATIC,
	DYNAMIC,
	UNDEFINED // for serialization helper functions
};
