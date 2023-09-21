/*!*************************************************************************
****
\file AssertException.cpp
\author Lor Xaun Yun Michelle
\par DP email: xaunyunmichelle.lor\@digipen.edu
\date 22-9-2022
\brief
This file handles try throw catch exception when it occurs.
It inherits from the exception library.
The difference between AssertException and LogException
is that AssertException will cause an assertion when exception
is caught while LogException does not. Both will then log to log
text file. AssertException handles fatal mode of exception,
LogException handles error mode of exception.

The functions
- GetLine
Is an accessor to the location where the exception was caused.

- GetFile
Is an accessor to the location of file where the exception was caused.

- ExceptionAssert
This function is used when exception is caught
****************************************************************************
***/
#include "../../../_TOOLS/include/pch.h"
#include "Debug/AssertException.h"

const int& AssertException::GetLine() const
{
	return m_line;
}

const std::string& AssertException::GetFile() const
{
	return m_file;
}

// Will cause assertion and print to log
// Marked as fatal
void AssertException::ExceptionAssert(const char* message, const std::string& file, const int& line)
{
	const char* f = file.c_str();
	std::string strLine = std::to_string(line);
	const char* l = strLine.c_str();
	PASSERTEXCEPTION(0, message, f, l);
}
