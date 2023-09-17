#pragma once

enum class MATERIAL : unsigned char
{
	RUBBER,
	WOOD,
	METAL,
	ICE,
	CONCRETE,
	GLASS
};

enum class MOTION : unsigned char
{
	STATIC,
	DYNAMIC
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