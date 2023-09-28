/*!*************************************************************************
****
\file LogException.cpp
\author Lor Xaun Yun Michelle
\par DP email: xaunyunmichelle.lor\@digipen.edu
\date 28-9-2023
\brief
This file handles try throw catch exception when it occurs.
It inherits from the exception library.

The functions
- GetLine
Is an accessor to the location where the exception was caused.

- GetFile
Is an accessor to the location of file where the exception was caused.

- ExceptionFound
This function is used when exception is caught
****************************************************************************
***/
#include "../../../_TOOLS/include/pch.h"
#include "Debug/LogException.h"

// Accessor
const int& LogException::GetLine() const
{
	return m_line;
}
const std::string& LogException::GetFile() const
{
	return m_file;
}

// Soley print to log
// Marked as error / warning (modes)
void LogException::ExceptionFound(const std::string& mode, const char* message, const std::string& file, const int& line)
{
	std::string strMessage(message);
	std::string strLine = std::to_string(line);
	std::string outputMessage = strMessage + "  " + file + "  Line: " + strLine;
	const char* finalOutput = outputMessage.c_str();
	PASSERTMSG(mode == "PERROR", "Please input the correct mode!");
	if (mode == "PERROR")
		PERROR(finalOutput);
}
