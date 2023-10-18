#pragma once
/*!*************************************************************************
****
\file		   InputMapSystem.h
\author(s)	   Cheong Ming Lun
\par DP email: m.cheong@digipen.edu
\date		   16-8-2023
\brief

This file contains the base InputMapSystem.h that supports mapping of inputs through
the editor to physical keys.
****************************************************************************/
#include "Input/Input.h"		// Input Keys

#define ADDITEM(x) map[E_KEYMapCounter++] = std::pair<std::string, E_KEY>(#x,x);

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

	/*!*****************************************************************************
	Remove a keybind from the system. If keybind does not exist, ignore.
	Then save it to file

	\param actionName
	- name of action
	*******************************************************************************/
	void RemoveKeybind(std::string const& actionName);

	/*!*****************************************************************************
	Save the keybind 
	*******************************************************************************/
	void SaveKeybind();

	/*!*****************************************************************************
	Getter for the list of keybind
	*******************************************************************************/
	std::map<std::string, E_KEY>* GetKeybindMap() { return &mKeybindMap; }

	/*!*****************************************************************************
	Getter for the list of available ekey's name and it's ekey enum
	*******************************************************************************/
	const std::array<std::pair<std::string, E_KEY>, 129>* GetEkeyMap() { return &mE_KEYMap; }

	std::string GetEKeyName(E_KEY ekey) { return std::find_if(mE_KEYMap.begin(), mE_KEYMap.end(), [ekey] (std::pair<std::string, E_KEY> item)->bool { return item.second == ekey; })->first; }

private:
	/*!*****************************************************************************
	Binding the E_KEY to it's direct name
	*******************************************************************************/
	constexpr static const std::array<std::pair<std::string, E_KEY>, 129> EKeyMappingInit() {
		int E_KEYMapCounter{};
		std::array<std::pair<std::string, E_KEY>, 129> map;
		ADDITEM(UNKNOWN);
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
	
	std::map<std::string, E_KEY> mKeybindMap; // Mapping action to E_KEY
	static const std::array<std::pair<std::string, E_KEY>, 129> mE_KEYMap; // Mapping E_KEY to it's name
};