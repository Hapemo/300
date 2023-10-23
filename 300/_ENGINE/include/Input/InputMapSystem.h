#pragma once
/*!*************************************************************************
****
\file					 InputMapSystem.h
\author(s)	   Cheong Ming Lun, Jazz Teoh Yu Jue
\par DP email: m.cheong@digipen.edu, j.teoh@digipen.edu
\date		   16-8-2023
\brief

This file contains the base InputMapSystem.h that supports mapping of inputs through
the editor to physical keys.
****************************************************************************/
#include "Input/Input.h"		// Input Keys

#define ADDITEM(x) map[E_KEYMapCounter++] = std::pair<std::string, E_KEY>(#x,x);
#define ADDITEMARR(x) arr[static_cast<int>(x)] = #x;

class InputMapSystem {
public:
	InputMapSystem();

	/*!*****************************************************************************
	Initialise the keybind using the keybind text file
	*******************************************************************************/
	void Init();

	/*!*****************************************************************************
	Checks the state of an action. *Current implementation has O(n)*, possible
	to use enum for optimisation later on when all actions are set and decided for
	O(1).

	\param actionName
	- Name of action

	\param state
	- key state

	\return bool
	- True if the state of key paired to action is triggered, otherwise false
	*******************************************************************************/
	bool CheckButton(std::string const& actionName, E_STATE state);

	/*!*****************************************************************************
	Check if the ekey linked to an action is hold.

	\param actionName
	- name of action
	*******************************************************************************/
	bool GetButton(std::string const& actionName);
	
	/*!*****************************************************************************
	Check if they ekey linked to an action is released.

	\param actionName
	- name of action
	*******************************************************************************/
	bool GetButtonUp(std::string const& actionName);
	
	/*!*****************************************************************************
	Check if the ekey linked to an action is pressed. Returns true for the first
	iteration, will not return true until button is released and pressed again

	\param actionName
	- name of action
	*******************************************************************************/
	bool GetButtonDown(std::string const& actionName);

	/*!*****************************************************************************
	Check if the ekey is hold.

	\param actionName
	- name of action
	*******************************************************************************/
	bool GetKey(E_KEY ekey);

	/*!*****************************************************************************
	Check if they ekey is released.

	\param actionName
	- name of action
	*******************************************************************************/
	bool GetKeyUp(E_KEY ekey);

	/*!*****************************************************************************
	Check if the ekey is pressed. Returns true for the first iteration, will not 
	return true until button is released and pressed again

	\param actionName
	- name of action
	*******************************************************************************/
	bool GetKeyDown(E_KEY ekey);

	//-----------------------------------------------------------------------------
	// Editor Only Functions
	//-----------------------------------------------------------------------------
	/*!*****************************************************************************
	Add a keybind to the system. If keybind already exists, replace the 
	existing one.
	Then save it to file

	\param actionName
	- name of action

	\param key
	- the glfw key
	*******************************************************************************/
	void AddKeybind(std::string const& actionName, E_KEY key);

	void AddEmptyKeybind(std::string const& actionName);

	/*!*****************************************************************************
	Remove a keybind from the system. If keybind does not exist, ignore.
	Then save it to file

	\param actionName
	- name of action
	*******************************************************************************/
	void RemoveKeybind(std::string const& actionName);

	/*!*****************************************************************************
	Save the keybind into external file. Also updates mKeybindMap based on
	mKeybindMapString
	*******************************************************************************/
	void SaveKeybind();

	/*!*****************************************************************************
	Getter for the list of keybind
	*******************************************************************************/
	std::map<std::string, E_KEY>* GetKeybindMap() { return &mKeybindMap; }

	/*!*****************************************************************************
	Getter for the list of keybind in string
	*******************************************************************************/
	std::map<std::string, std::string >* GetKeybindMapString() { return &mKeybindMapString; }

	///*!*****************************************************************************
	//Getter for the list of available ekey's name and it's ekey enum
	//*******************************************************************************/
	//const std::array<std::pair<std::string, E_KEY>, 129>* GetEkeyMap() { return &mE_KEYMap; }
	
	/*!*****************************************************************************
	Getter for the list of available ekey's name and it's ekey enum in array
	*******************************************************************************/
	const std::array<std::string, E_KEY::KEY_TOTAL>* GetEkeyArray() { return &mE_KEYArray; }

	std::string GetEKeyName(E_KEY ekey);
	E_KEY GetEKeyFromName(std::string const& ekeyName);
	std::string& GetActionEKeyName(std::string const& action);

private:
	/*!*****************************************************************************
	Binding the E_KEY to it's direct name
	*******************************************************************************/
	constexpr static const std::array<std::pair<std::string, E_KEY>, 129> EKeyMappingInit() {
		int E_KEYMapCounter{};
		std::array<std::pair<std::string, E_KEY>, 129> map;
		ADDITEM(ERROR_EKEY);
		ADDITEM(SPACE);
		ADDITEM(APOSTROPHE);
		ADDITEM(COMMA);
		ADDITEM(MINUS);
		ADDITEM(PERIOD);
		ADDITEM(SLASH);
		ADDITEM(_0);
		ADDITEM(_1);
		ADDITEM(_2);
		ADDITEM(_3);
		ADDITEM(_4);
		ADDITEM(_5);
		ADDITEM(_6);
		ADDITEM(_7);
		ADDITEM(_8);
		ADDITEM(_9);
		ADDITEM(SEMICOLON);
		ADDITEM(EQUAL);
		ADDITEM(A);
		ADDITEM(B);
		ADDITEM(C);
		ADDITEM(D);
		ADDITEM(E);
		ADDITEM(F);
		ADDITEM(G);
		ADDITEM(H);
		ADDITEM(I);
		ADDITEM(J);
		ADDITEM(K);
		ADDITEM(L);
		ADDITEM(M);
		ADDITEM(N);
		ADDITEM(O);
		ADDITEM(P);
		ADDITEM(Q);
		ADDITEM(R);
		ADDITEM(S);
		ADDITEM(T);
		ADDITEM(U);
		ADDITEM(V);
		ADDITEM(W);
		ADDITEM(X);
		ADDITEM(Y);
		ADDITEM(Z);
		ADDITEM(LEFT_BRACKET);
		ADDITEM(BACKSLASH);
		ADDITEM(RIGHT_BRACKET);
		ADDITEM(GRAVE_ACCENT);
		ADDITEM(WORLD_1);
		ADDITEM(WORLD_2);
		ADDITEM(ESCAPE);
		ADDITEM(ENTER);
		ADDITEM(TAB);
		ADDITEM(BACKSPACE);
		ADDITEM(INSERT);
		ADDITEM(DELETE);
		ADDITEM(RIGHT);
		ADDITEM(LEFT);
		ADDITEM(DOWN);
		ADDITEM(UP);
		ADDITEM(PAGE_UP);
		ADDITEM(PAGE_DOWN);
		ADDITEM(HOME);
		ADDITEM(END);
		ADDITEM(CAPS_LOCK);
		ADDITEM(SCROLL_LOCK);
		ADDITEM(NUM_LOCK);
		ADDITEM(PRINT_SCREEN);
		ADDITEM(PAUSE);
		ADDITEM(F1);
		ADDITEM(F2);
		ADDITEM(F3);
		ADDITEM(F4);
		ADDITEM(F5);
		ADDITEM(F6);
		ADDITEM(F7);
		ADDITEM(F8);
		ADDITEM(F9);
		ADDITEM(F10);
		ADDITEM(F11);
		ADDITEM(F12);
		ADDITEM(F13);
		ADDITEM(F14);
		ADDITEM(F15);
		ADDITEM(F16);
		ADDITEM(F17);
		ADDITEM(F18);
		ADDITEM(F19);
		ADDITEM(F20);
		ADDITEM(F21);
		ADDITEM(F22);
		ADDITEM(F23);
		ADDITEM(F24);
		ADDITEM(F25);
		ADDITEM(PAD_0);
		ADDITEM(PAD_1);
		ADDITEM(PAD_2);
		ADDITEM(PAD_3);
		ADDITEM(PAD_4);
		ADDITEM(PAD_5);
		ADDITEM(PAD_6);
		ADDITEM(PAD_7);
		ADDITEM(PAD_8);
		ADDITEM(PAD_9);
		ADDITEM(PAD_DECIMAL);
		ADDITEM(PAD_DIVIDE);
		ADDITEM(PAD_MULTIPLY);
		ADDITEM(PAD_SUBTRACT);
		ADDITEM(PAD_ADD);
		ADDITEM(PAD_ENTER);
		ADDITEM(PAD_EQUAL);
		ADDITEM(LEFT_SHIFT);
		ADDITEM(LEFT_CONTROL);
		ADDITEM(LEFT_ALT);
		ADDITEM(LEFT_SUPER);
		ADDITEM(RIGHT_SHIFT);
		ADDITEM(RIGHT_CONTROL);
		ADDITEM(RIGHT_ALT);
		ADDITEM(RIGHT_SUPER);
		ADDITEM(MENU);
		ADDITEM(M_BUTTON_L);
		ADDITEM(M_BUTTON_R);
		ADDITEM(M_BUTTON_M);
		ADDITEM(M_BUTTON_4);
		ADDITEM(M_BUTTON_5);
		ADDITEM(M_BUTTON_6);
		ADDITEM(M_BUTTON_7);
		ADDITEM(M_BUTTON_8);
		return map;
	}

	constexpr static const std::array<std::string, E_KEY::KEY_TOTAL> EKeyArrayInit() {
		std::array<std::string, E_KEY::KEY_TOTAL> arr;
		ADDITEMARR(ERROR_EKEY);
		ADDITEMARR(SPACE);
		ADDITEMARR(APOSTROPHE);
		ADDITEMARR(COMMA);
		ADDITEMARR(MINUS);
		ADDITEMARR(PERIOD);
		ADDITEMARR(SLASH);
		ADDITEMARR(_0);
		ADDITEMARR(_1);
		ADDITEMARR(_2);
		ADDITEMARR(_3);
		ADDITEMARR(_4);
		ADDITEMARR(_5);
		ADDITEMARR(_6);
		ADDITEMARR(_7);
		ADDITEMARR(_8);
		ADDITEMARR(_9);
		ADDITEMARR(SEMICOLON);
		ADDITEMARR(EQUAL);
		ADDITEMARR(A);
		ADDITEMARR(B);
		ADDITEMARR(C);
		ADDITEMARR(D);
		ADDITEMARR(E);
		ADDITEMARR(F);
		ADDITEMARR(G);
		ADDITEMARR(H);
		ADDITEMARR(I);
		ADDITEMARR(J);
		ADDITEMARR(K);
		ADDITEMARR(L);
		ADDITEMARR(M);
		ADDITEMARR(N);
		ADDITEMARR(O);
		ADDITEMARR(P);
		ADDITEMARR(Q);
		ADDITEMARR(R);
		ADDITEMARR(S);
		ADDITEMARR(T);
		ADDITEMARR(U);
		ADDITEMARR(V);
		ADDITEMARR(W);
		ADDITEMARR(X);
		ADDITEMARR(Y);
		ADDITEMARR(Z);
		ADDITEMARR(LEFT_BRACKET);
		ADDITEMARR(BACKSLASH);
		ADDITEMARR(RIGHT_BRACKET);
		ADDITEMARR(GRAVE_ACCENT);
		ADDITEMARR(WORLD_1);
		ADDITEMARR(WORLD_2);
		ADDITEMARR(ESCAPE);
		ADDITEMARR(ENTER);
		ADDITEMARR(TAB);
		ADDITEMARR(BACKSPACE);
		ADDITEMARR(INSERT);
		ADDITEMARR(DELETE);
		ADDITEMARR(RIGHT);
		ADDITEMARR(LEFT);
		ADDITEMARR(DOWN);
		ADDITEMARR(UP);
		ADDITEMARR(PAGE_UP);
		ADDITEMARR(PAGE_DOWN);
		ADDITEMARR(HOME);
		ADDITEMARR(END);
		ADDITEMARR(CAPS_LOCK);
		ADDITEMARR(SCROLL_LOCK);
		ADDITEMARR(NUM_LOCK);
		ADDITEMARR(PRINT_SCREEN);
		ADDITEMARR(PAUSE);
		ADDITEMARR(F1);
		ADDITEMARR(F2);
		ADDITEMARR(F3);
		ADDITEMARR(F4);
		ADDITEMARR(F5);
		ADDITEMARR(F6);
		ADDITEMARR(F7);
		ADDITEMARR(F8);
		ADDITEMARR(F9);
		ADDITEMARR(F10);
		ADDITEMARR(F11);
		ADDITEMARR(F12);
		ADDITEMARR(F13);
		ADDITEMARR(F14);
		ADDITEMARR(F15);
		ADDITEMARR(F16);
		ADDITEMARR(F17);
		ADDITEMARR(F18);
		ADDITEMARR(F19);
		ADDITEMARR(F20);
		ADDITEMARR(F21);
		ADDITEMARR(F22);
		ADDITEMARR(F23);
		ADDITEMARR(F24);
		ADDITEMARR(F25);
		ADDITEMARR(PAD_0);
		ADDITEMARR(PAD_1);
		ADDITEMARR(PAD_2);
		ADDITEMARR(PAD_3);
		ADDITEMARR(PAD_4);
		ADDITEMARR(PAD_5);
		ADDITEMARR(PAD_6);
		ADDITEMARR(PAD_7);
		ADDITEMARR(PAD_8);
		ADDITEMARR(PAD_9);
		ADDITEMARR(PAD_DECIMAL);
		ADDITEMARR(PAD_DIVIDE);
		ADDITEMARR(PAD_MULTIPLY);
		ADDITEMARR(PAD_SUBTRACT);
		ADDITEMARR(PAD_ADD);
		ADDITEMARR(PAD_ENTER);
		ADDITEMARR(PAD_EQUAL);
		ADDITEMARR(LEFT_SHIFT);
		ADDITEMARR(LEFT_CONTROL);
		ADDITEMARR(LEFT_ALT);
		ADDITEMARR(LEFT_SUPER);
		ADDITEMARR(RIGHT_SHIFT);
		ADDITEMARR(RIGHT_CONTROL);
		ADDITEMARR(RIGHT_ALT);
		ADDITEMARR(RIGHT_SUPER);
		ADDITEMARR(MENU);
		ADDITEMARR(M_BUTTON_L);
		ADDITEMARR(M_BUTTON_R);
		ADDITEMARR(M_BUTTON_M);
		ADDITEMARR(M_BUTTON_4);
		ADDITEMARR(M_BUTTON_5);
		ADDITEMARR(M_BUTTON_6);
		ADDITEMARR(M_BUTTON_7);
		ADDITEMARR(M_BUTTON_8);
		return arr;
	}

	std::map<std::string, E_KEY> mKeybindMap; // Mapping action to E_KEY
	std::map<std::string, std::string> mKeybindMapString; // Mapping action to E_KEY's name (for imgui to print only)
	//static const std::array<std::pair<std::string, E_KEY>, 129> mE_KEYMap;	// Mapping E_KEY to it's name
	static const std::array<std::string, E_KEY::KEY_TOTAL> mE_KEYArray;		// Mapping E_KEY's name to it's index in array
};