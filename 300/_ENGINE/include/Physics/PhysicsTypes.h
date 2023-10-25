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
	STATIC = static_cast<unsigned char>(1U << 0),
	DYNAMIC = static_cast<unsigned char>(1U << 1),
	UNDEFINED // for serialization helper functions
};

// please update the maps here
static std::unordered_map<std::string, MATERIAL> MaterialMap = { {"Rubber", MATERIAL::RUBBER},
																 {"Wood", MATERIAL::WOOD},
																 {"Metal", MATERIAL::METAL},
																 {"Ice", MATERIAL::ICE},
																 {"Concrete", MATERIAL::CONCRETE},
																 {"Glass", MATERIAL::GLASS} };

static std::unordered_map<std::string, MOTION> MotionMap = { {"Static", MOTION::STATIC},
															 {"Dynamic", MOTION::DYNAMIC} };
