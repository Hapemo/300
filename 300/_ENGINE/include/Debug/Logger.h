/*!*************************************************************************
****
\file Logger.h
\author Lor Xaun Yun Michelle
\par DP email: xaunyunmichelle.lor\@digipen.edu
\date 22-9-2022
\brief
This file contains data regarding the logger.
It consists of 5 modes of logging:
PERROR: error logs
PWARNING: warnings that will put the program at risk in the future
if not solved
PINFO: information whne the program hits a certain mark
(for example: when number of entities reach 100)
PDEBUG: to log code that requires debugging
PTRACE: to aid in debugging

The functions
- InitLogging
Creates a log text file base on the current date and time that the
program started running. Open the text file created for logging.

- LogOutput
Called by all macros to log mode of logging and custom message
into log text file. The deque container will also be called here
to store the same log for printing in the imGUI logger console.

- EndLogging
Called when assertions is thrown or when window closes. This
function will clear the deque container containing all existing
logs and close the log text file.

- EntityCountLog
Will log to log text file when a certain set of entities had been
created in the program. (100, 300, 500, 700, 1000 entities).

NEW UPDATE:
Able to turn off logs in release mode (Logger window in 
ImGui will be empty)
****************************************************************************
***/
#pragma once
#include <fstream>
#include <deque>
#include <regex>
#include<stdarg.h>
#include<stdio.h>

//disable debug and trace modes on release
#if _DEBUG 
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1
#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#else
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#define LOG_WARN_ENABLED 0
#define LOG_INFO_ENABLED 0
#endif

typedef enum log_mode
{
	LOG_LEVEL_FATAL = 0,
	LOG_LEVEL_ERROR = 1,
	LOG_LEVEL_WARN = 2,
	LOG_LEVEL_INFO = 3,
	LOG_LEVEL_DEBUG = 4,
	LOG_LEVEL_TRACE = 5
} log_mode;

/******************************************************************************/
/*!
	Logger Class
 */
 /******************************************************************************/
class Logger
{
public:
	static std::ofstream Pfile;
	static std::deque<std::string> logToGUI;
	//firstTimeFLag is used in loggerWindow.cpp
	//flag is used in loggerWindow.cpp
	static bool firstTimeFlag, flag, concatFlag;
	/***************************************************************************/
	/*!
	\brief
	This function initializes logger.
	Creates a file based on the current date and time.
	Opens the text file for logging throughout the program.
	*/
	/**************************************************************************/
	static void InitLogging();
	
	/***************************************************************************/
	/*!
	\brief
	This function outputs log(s) into the log text file.
	
	\param mode
	The different modes of logging.
	(FATAL, ERROR, WARNING, INFO, DEBUG, TRACE)
	
	\param message
	Custom message input by developers to be printed to
	the log text file.
	*/
	/**************************************************************************/
	static void LogOutput(log_mode mode, const char* message, ...);

	/***************************************************************************/
	/*!
	\brief
	This function ends the process of logging by clearing the deque
	container that stores the logs that have been created throughout
	the program and closing the log text file.
	*/
	/**************************************************************************/
	static void EndLogging();

	/***************************************************************************/
	/*!
	\brief
	This function triggers PINFO log to print to the log text file
	when a certain amount of entities created had been achieved.
	
	\param entityCount
	Takes in a reference to entityCoint variable from ECS.
	(entityCount variable from ECS increments everytime an entity
	is created.)
	*/
	/**************************************************************************/
	static void EntityCountLog(int& entityCount);
};

//logs fatal mode message (always enabled and available)
#define PFATAL(message, ...) Logger::LogOutput(LOG_LEVEL_FATAL, message, ##__VA_ARGS__);
//logs error mode message (always enabled and available) (turn of for release mode)
#define PERROR(message, ...) Logger::LogOutput(LOG_LEVEL_ERROR, message, ##__VA_ARGS__);

//logs warning mode message
#if LOG_WARN_ENABLED == 1
#define PWARNING(message, ...) Logger::LogOutput(LOG_LEVEL_WARN, message, ##__VA_ARGS__);
#else
#define PWARNING(message, ...)
#endif

//logs info mode message
#if LOG_INFO_ENABLED == 1
#define PINFO(message, ...) Logger::LogOutput(LOG_LEVEL_INFO, message, ##__VA_ARGS__);
#else
#define PINFO(message, ...)
#endif

//logs debug mode message
#if LOG_DEBUG_ENABLED == 1
#define PDEBUG(message, ...) Logger::LogOutput(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__);
#else
#define PDEBUG(message, ...)
#endif

//logs trace mode message
#if LOG_TRACE_ENABLED == 1
#define PTRACE(message, ...) Logger::LogOutput(LOG_LEVEL_TRACE, message, ##__VA_ARGS__);
#else
#define PTRACE(message, ...)
#endif
