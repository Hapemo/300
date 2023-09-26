#pragma once
#include <string>

typedef void (*FunctionPointer)(bool);

struct Subscriber {
	std::string name;
	FunctionPointer function_ptr;
};