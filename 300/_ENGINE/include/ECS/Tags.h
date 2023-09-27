#pragma once
#include "pch.h"

enum class TAG : unsigned char
{
	PLAYER,
	UNKNOWN,
	UNDEFINED // for serialization helper functions
	//more to come
};

enum class SUBTAG : unsigned char
{
	ACTIVE,
	BACKGROUND,
	UNDEFINED // for serialization helper functions
	//more to come
};

// every time tags/subtags get updated, please update the maps here
static std::unordered_map<std::string, TAG> TagMap = { {"Player", TAG::PLAYER},
													   {"Unknown", TAG::UNKNOWN} };

static std::unordered_map<std::string, SUBTAG> SubTagMap = { {"Active", SUBTAG::ACTIVE},
															 {"Background", SUBTAG::BACKGROUND} };