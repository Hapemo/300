/*!*************************************************************************
****
\file AssertException.h
\author Lor Xaun Yun Michelle
\par DP email: xaunyunmichelle.lor\@digipen.edu
\date 28-9-2023
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
#pragma once
#include "Asserts.h"
#include "Logger.h"

#define EXCEPTIONASSERT_ENABLED 1

/*******************************/
/*!
	AssertException Class
*/
/*******************************/
class AssertException : public std::exception
{
public:
	AssertException(const std::string& message, const std::string& file, int line)
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

	// Will cause assertion and print to log, marked as fatal
	/***************************************************************************/
	/*!
	\brief
	This function handles the message and mode to log after exception had
	been caught.

	\param &mode
	Log mode of exception to be printed to log.
	(Will be in fatal mode as PASSERTEXCEPTION macro defined in Assert.h
	will be called)

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
	static void ExceptionAssert(const char* message, const std::string& file, const int& line);
};

#if EXCEPTIONASSERT_ENABLED == 1
#define PEXCEPTIONASSERT(message) throw AssertException(message, __FILE__, __LINE__);
#else
#define PEXCEPTIONASSERT(message)
#endif
