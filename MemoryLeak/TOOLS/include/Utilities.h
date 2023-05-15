/*!*****************************************************************************
\file Utilities.h
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 26-09-2022
\brief
This file contains the class Util and it's functions declaration.
The functions in this class acts as an aid to other functions.
*******************************************************************************/
#pragma once
#include "pch.h"

#define ALPHA "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

class Util {
public:
    enum class E_DTFORMAT {
        DATE_TIME = 0,
        DATE = 1,
        TIME = 2
    };

    /*!*****************************************************************************
    \brief
    Get current date and time in DD-MM-YYYY | HH:mm:ss format

    \param std::string _input
    The string to be converted.

    \return
    Returns a const date and time in string.
    *******************************************************************************/
    static const std::string CurrentDateTime(E_DTFORMAT _format);

    /*!*****************************************************************************
    \brief
    Converts the input string into a full uppercase string.

    \param std::string _input
    The string to be converted.

    \return
    Returns a const of the uppercase transformed string.
    *******************************************************************************/
    static const std::string UpperString(std::string _input);

    /*!*****************************************************************************
    \brief
    Converts the input string into a full lowercase string.

    \param std::string _input
    The string to be converted.

    \return
    Returns a const of the lowercase transformed string.
    *******************************************************************************/
    static const std::string LowerString(std::string _input);

    /*!*****************************************************************************
    \brief
    Checking for memory leak.
    *******************************************************************************/
    static void EnableMemoryLeakChecking(int breakAlloc = -1);

    template <typename T>
    static const bool IsWholeNum(T _num) {
        return (_num - floor(_num)) != 0 ? false : true;
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
    static std::map<std::string, std::string> TextFileToMap(std::fstream& file);

    // Make a static number
    static inline int RandInt(int max, int min) { return (rand() % (max - min + 1)) + min;  }

    /*
    static double RoundToDecimal(double _value, int _decimal) {
        if (_decimal < 0) return _value;
        int decimalPlace = 1;
        std::string decimalPlaceStr = std::to_string(decimalPlace);
        for (int i = 0; i < _decimal; ++i) {
            decimalPlaceStr += '0';
        }
        printf("%f\n", roundf(_value * std::stoi(decimalPlaceStr)) / std::stoi(decimalPlaceStr));
        return roundf(_value * std::stoi(decimalPlaceStr)) / std::stoi(decimalPlaceStr);
    }
    */
};
