#pragma once
#include <string>
#include <sstream>
#include <algorithm>

static void SplitEnum(const char* arg, std::string* Array, int count)
{
	std::stringstream ss{ arg };
	std::string string;

	int index = 0;

	while (ss.good() && index < count)
	{
		getline(ss, Array[index], ',');

		for (auto iter = Array[index].begin(); iter != Array[index].end(); ++iter)
		{
			// Remove the whitespace
			if (*iter == ' ')
			{
				iter = Array[index].erase(iter);
				if (iter != Array[index].begin())
					--iter;
			}
			// Replace the underscore with whitespace
			else if (*iter == '_')
			{
				*iter = ' ';
			}
			else if (iter != Array[index].begin())
			{
				auto previous = iter - 1;

				// Check if it is not the first letter of the word and if the letter is an uppercase
				if (*previous != ' ' && *iter >= 'A' && *iter <= 'Z')
				{
					// Convert it to lowercase
					*iter += 32;
				}
			}
		}

		++index;
	}
}

// ENUM_NAME refers to the name of the type. Put the enum variables after ENUM_NAME
#define DECLARE_ENUMSTRING(ENUM_NAME, ...)\
		namespace ENUM_NAME\
		{\
			enum ENUM_NAME\
			{\
				__VA_ARGS__, COUNT\
			};\
			\
			static std::string EnumString[COUNT];\
			\
			static const char* ToString(ENUM_NAME _enum)\
			{\
				if (EnumString[0].empty())\
				{\
					SplitEnum(#__VA_ARGS__, EnumString, COUNT);\
				}\
				\
				return EnumString[_enum].c_str();\
			}\
			\
			static ENUM_NAME GetEnum(const char* name)\
			{\
				if (EnumString[0].empty())\
				{\
					SplitEnum(#__VA_ARGS__, EnumString, COUNT);\
				}\
				\
				for (int i = 0; i < COUNT; ++i)\
				{\
					if (strcmp(name, EnumString[i].c_str()) == 0)\
					{\
						return static_cast<ENUM_NAME>(i);\
					}\
				}\
				\
				return COUNT;\
			}\
		}
