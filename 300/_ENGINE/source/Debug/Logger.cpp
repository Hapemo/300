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
#include "../../../_TOOLS/include/pch.h"
#include "Debug/Logger.h"

std::ofstream Logger::Pfile{};
std::deque<std::string> Logger::logToGUI;
bool Logger::flag = false;
bool Logger::firstTimeFlag = true;

void Logger::InitLogging()
{
	//Get current date to name Log file
	auto curr = std::chrono::system_clock::now();
	std::time_t currTime = std::chrono::system_clock::to_time_t(curr);

	//Generating the appropriate name for the file 
	char timeStr[26];
	ctime_s(timeStr, sizeof(timeStr), &currTime);
	std::string temp2 = timeStr;
	temp2 = std::regex_replace(temp2, std::regex(" "), "_");
	temp2 = std::regex_replace(temp2, std::regex(":"), "_");

	//Concatenating the name with the appropriate file location
	std::stringstream ss;
	std::string log = "Log.txt";
	//ss << "../../ENGINE/Resources/Logs/" << temp2 << log;
	ss << "../Logs/" << log;
	std::string temp = ss.str();
	temp.erase(std::remove(temp.begin(), temp.end(), '\n'), temp.cend());
	const char* path = temp.c_str();

	//Create log text file and open for writing
	Pfile.open(path, std::ios_base::out);
}

void Logger::LogOutput(log_mode mode, const char* message, ...)
{
	//6 different modes of logs
	const char* mode_strings[6] = { "{FATAL}: ", "{ERROR}: ", "{WARN}: ",
									"{INFO}: ", "{DEBUG}: ", "{TRACE}: " };

	//Sets a 1k character limit on each log entry
	char outputMessage[1000];
	memset(outputMessage, 0, sizeof(outputMessage));

	//Making use of <stdarg.h> library that accepts an indefinite number of mesages
	va_list argument_ptr;
	va_start(argument_ptr, message);
	vsnprintf(outputMessage, 1000, message, argument_ptr);
	va_end(argument_ptr);

	//Get current date to name Log file
	auto curr = std::chrono::system_clock::now();
	std::time_t currTime = std::chrono::system_clock::to_time_t(curr);

	//Generating the appropriate name for the file 
	char timeStr[26];
	ctime_s(timeStr, sizeof(timeStr), &currTime);
	std::string temp2 = timeStr;
	temp2 = std::regex_replace(temp2, std::regex(" "), "_");
	temp2 = std::regex_replace(temp2, std::regex(":"), "_");

	//Output to opened log file from InitLogging()
	Pfile << temp2 << mode_strings[mode] << outputMessage << "\n";
	Pfile << "\n";
	std::string strMode(mode_strings[mode]);
	std::string strMessage(outputMessage);
	flag = true;
	//Store log in std::deque container that will be used to
	//output log to imGUI logging panel
	logToGUI.push_back(strMode + strMessage + "\n");
}

void Logger::EndLogging()
{
	//Clear all logs stored in std::deque container
	logToGUI.clear();
	//Close the log file from InitLogging()
	Pfile.close();
}

void Logger::EntityCountLog(int& entityCount)
{
	if (entityCount == 101 || entityCount == 301 || entityCount == 501 || entityCount == 701 || entityCount == 1001)
	{
		PINFO("Entity count had reached: %d.", entityCount-1)
	}
}

void reportAssertion(const char* expression, const char* message, const char* file, int line)
{
	//when assertion detected log to file as FATAL
	Logger::LogOutput(LOG_LEVEL_FATAL, "Assertion Failure : %s, Message: '%s', IN FILE: %s, AT LINE: %d\n", expression, message, file, line);
	Logger::EndLogging();
}

void reportFatalException(const char* message, const char* file, const char* line)
{
	//when exception is thrown log to file as FATAL
	Logger::LogOutput(LOG_LEVEL_FATAL, "Exception Failure : Message: '%s', IN FILE: %s, AT LINE: %s\n", message, file, line);
	Logger::EndLogging();
}
