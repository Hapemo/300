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