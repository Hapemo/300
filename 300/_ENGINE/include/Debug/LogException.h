/*!*************************************************************************
****
\file LogException.h
\author Lor Xaun Yun Michelle
\par DP email: xaunyunmichelle.lor\@digipen.edu
\date 28-09-2023
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

- ExceptionFound
This function is used when exception is caught
****************************************************************************
***/
#pragma once
#include "Asserts.h"
#include "Logger.h"

#define EXCEPTIONFOUND_ENABLED 1

/*******************************/
/*!
	LogException Class
*/
/*******************************/
class LogException : public std::exception
{
public:
	LogException(const std::string& message, const std::string& file, int line)
		: m_message(message), m_file(file), m_line(line) {};

private:
	std::string m_message{};
	// Called before throwing exception
	std::string m_file;
	int m_line;

public:
	/***************************************************************************/
	/*!
	\brief
	This function overrides what() virtual function from std::exception
	*/
	/**************************************************************************/
	const char* what() const noexcept override
	{
		return m_message.c_str();
	}
	// Accessor
	const int& GetLine() const;
	const std::string& GetFile() const;

	// Soley print to log, marked as error mode
	/***************************************************************************/
	/*!
	\brief
	This function handles the message and mode to log after exception had
	been caught.
	
	\param &mode
	Log mode of exception to be printed to log.
	(Have to be error mode, if not assertion will be activated)
	
	\param message
	Custom message of exception to be printed to log.
	
	\param file
	File where exception had been thrown, will be logged so
	developers are able to easily find where the error had occured.
	
	\param line
	Line of code where exception had been thrown, will be logged so
	developers are able to easily find where the error had occured.
	*/
	/**************************************************************************/
	static void ExceptionFound(const std::string& mode, const char* message, const std::string& file, const int& line);
};

/***************************************************************************/
/*!
\brief
MACRO to enable exception to be thrown
*/
/**************************************************************************/
#if EXCEPTIONFOUND_ENABLED == 1
#define PEXCEPTIONFOUND(message) throw LogException(message, __FILE__, __LINE__);
#else
#define PEXCEPTIONFOUND(message)
#endif