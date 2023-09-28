#pragma once

enum MAP_TO_AXIS
{
	X_POSITIVE,
	Y_POSITIVE,
	X_NEGATIVE,
	Y_NEGATIVE,
	EMPTY
};


enum KEY_BIND
{
	KEY_UP,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_DOWN,
	KEY_BIND_TOTAL
};

// Let's Start with these keys for now...
static std::string user_key[] =
{	
	" " // Unknown
	"(SPACE) _",
	"'", /* ' */
	",", /* , */
	".",
	"-",
	"/",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	":", // Semicolon
	"\\", // Backslash
	"=",
	"A", "B" , "C" , "D" , "E", "F", "G", "H", "I" , "J", "K", "L", "M", "N", "O", "P" , "Q" , "R",
	"S", "T" , "U" , "V" , "W" , "X" , "Y" , "Z",
	"UP", "DOWN" , "LEFT" , "RIGHT"
};


