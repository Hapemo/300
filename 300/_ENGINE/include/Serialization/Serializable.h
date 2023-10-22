#pragma once
#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"

struct Serializable
{
	virtual void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const = 0;
	virtual void DeserializeSelf(rapidjson::Value& reader) = 0;
};