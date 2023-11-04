/*!*************************************************************************
****
\file		   SerializeSelf.cpp
\author(s)	   Charissa Yip, Kew Yu Jun
\par DP email:
charissa.yip\@digipen.edu
\date		   11-4-2023
\brief
This file contains definitions of functions to be used in each component class.
****************************************************************************/
#pragma once
#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"

/***************************************************************************/
/*!
\brief
Serializable struct that every component will inherit from so the component
gets to be serialized and deserialized.

SerializeSelf
\param writer
The writer to write the data into for JSON to save the components into a
JSON file.

DeserializeSelf
\param reader
The reader to read the JSON data from to initialize the components.

*/
/***************************************************************************/
struct Serializable
{
	virtual void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const = 0;
	virtual void DeserializeSelf(rapidjson::Value& reader) = 0;
};