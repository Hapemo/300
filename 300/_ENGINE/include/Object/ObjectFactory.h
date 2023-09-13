#pragma once
#include "pch.h"
#include "../Serialization/Serialization.h"

class ObjectFactory
{
public:
	static void DeserializeScene(const std::string& filename);
	static void SerializeScene(const std::string& filename);
};