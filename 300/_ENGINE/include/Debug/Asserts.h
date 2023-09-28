/*!*************************************************************************
****
\file Asserts.h
\author Lor Xaun Yun Michelle
\par DP email: xaunyunmichelle.lor\@digipen.edu
\date 28-09-2023
\brief
This file contains the assertion MACROS that can be used.
All of them will result in __debugbreak(), where a breakpoint
will occur.

The functions
- reportAssertion
Defined in Logger.cpp.

- reportFatalException
Defined in Logger.cpp.
****************************************************************************
***/
#pragma once

#if _MSC_VER
#include <intrin.h>
#define debugBreak() __debugbreak()
#else
#define debugBreak() __builtin_trap()
#endif

/***************************************************************************/
/*!
\brief
This function triggers PFATAL assertion (__debugbreak() will be called)
when the program triggers an assertion due to a false expression.
It will then log to the log text file and logging process will end.

\param expression
Code that caused the assertion.

\param message
The custom message for the assertion that will be printed out.

\param file
The name of the file that the assertion was triggered.

\param line
The line where the assertion was triggered.
*/
/**************************************************************************/
void reportAssertion(const char* expression, const char* message, const char* file, int line);

/***************************************************************************/
/*!
\brief
This function logs to the log text file when an exception is triggered.

Reason why the function reportAssertion() was not used instead:
We need to pass in the __LINE__ and __FILE__ as params
calling reportAssertion() will not provide us with the actual
line and file that we need as it will not be triggered at the
actual line that throws the exception.

\param message
The custom message for the  that will be printed out.

\param file
The name of the file that the assertion was triggered.

\param line
The line where the assertion was triggered.
*/
/**************************************************************************/
void reportFatalException(const char* message, const char* file, const char* line);

#define PASSERT(expr)													          \
{																		          \
	if (expr)															          \
	{ }																			  \
	else																		  \
	{																			  \
		reportAssertion(#expr, "", __FILE__, __LINE__);			  \
		debugBreak();															  \
	}																			  \
}																				  \

#define PASSERTMSG(expr, message)                                                 \
{                                                                                 \
	if (expr)                                                                     \
	{                                                                             \
	}                                                                             \
	else                                                                          \
	{                                                                             \
		reportAssertion(#expr, message, __FILE__, __LINE__);       \
		debugBreak();                                                             \
	}                                                                             \
}                                                                                 \

#define PASSERTEXCEPTION(expr, message, file, line)                               \
{																				   \
	if (!expr)                                                                     \
	{                                                                              \
		reportFatalException(message, file, line);                                 \
		debugBreak();                                                              \
	}                                                                              \
}                                                                                  \

#ifdef DEBUG                                                                      
#define PASSERT_DEBUG(expr)                                                       \
	{                                                                             \
		if (expr)                                                                 \
		{ }                                                                       \
		else                                                                      \
		{                                                                         \
			reportAssertion(#expr, "", __FILE__, __LINE__);        \ 
			debugBreak();                                                         \
		}                                                                         \
	}                                                                             \
		            
#else
//Does nothing
#define PASSERTDEBUG(expr)
#endif