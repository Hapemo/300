/*!*****************************************************************************
\file Utilities.cpp
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 26-09-2022
\brief
This file contains the function definitions of the class Util.
The functions in this class acts as an aid to other functions.
*******************************************************************************/

#include "Utilities.h"

/*!*****************************************************************************
\brief
Get current date and time in DD-MM-YYYY | HH:mm:ss format

\param std::string _input
The string to be converted.

\return
Returns a const date and time in string.
*******************************************************************************/
const std::string Util::CurrentDateTime(Util::E_DTFORMAT _format) {
    time_t now = time(NULL);
    char datetime[80];
    struct tm timeStruct;
    localtime_s(&timeStruct, &now);
    strftime(datetime, sizeof(datetime), (_format == E_DTFORMAT::DATE ? "%d-%m-%Y" : (_format == E_DTFORMAT::TIME ? "%X" : "%d-%m-%Y %X")), &timeStruct);
    return datetime;
}

/*!*****************************************************************************
\brief
Converts the input string into a full uppercase string.

\param std::string _input
The string to be converted.

\return
Returns a const of the uppercase transformed string.
*******************************************************************************/
const std::string Util::UpperString(std::string _input) {
    std::string result;
    for (char letter : _input) {
        result += (char)(toupper(letter));
    }
    return result;
}

/*!*****************************************************************************
\brief
Converts the input string into a full lowercase string.

\param std::string _input
The string to be converted.

\return
Returns a const of the lowercase transformed string.
*******************************************************************************/
const std::string Util::LowerString(std::string _input) {
    std::string result;
    for (char letter : _input) {
        result += (char)(tolower(letter));
    }
    return result;
}

/*!*****************************************************************************
\brief
Load a text file into a map. The format in text file should be as follows:
<key1>: <value1>\n
<key2>: <value2>\n
<key3>: <value3>\n
<key4>: <value4>\n

\param std::fstream& file
- Input file

\return std::map<std::string, std::string>
- Map containing data extracted from file
*******************************************************************************/
std::map<std::string, std::string> Util::TextFileToMap(std::fstream& _file) {
    std::map<std::string, std::string> data;
    std::string line{};
    while (std::getline(_file, line)) {
        size_t midpoint{ line.find(':') };
        data.insert({ line.substr(0, midpoint), line.substr(midpoint + 2, midpoint + 2 - line.length()) });
    }
    return data;
}

/*!*****************************************************************************
\brief
Checking for memory leak.
*******************************************************************************/
void Util::EnableMemoryLeakChecking(int breakAlloc)
{
    //Set the leak checking flag
    int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(tmpDbgFlag);

    //If a valid break alloc provided set the breakAlloc
    if (breakAlloc != -1) _CrtSetBreakAlloc(breakAlloc);
}
