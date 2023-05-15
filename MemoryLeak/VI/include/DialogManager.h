/*!*****************************************************************************
\file DialogManager.h
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 26-09-2022
\brief
This file contains the class DialogManager and it's functions declaration.
The DialogManager manages the scripts and dialogues data.
*******************************************************************************/

#pragma once
#include <ECS_systems.h>
#include "ECS_items.h"
#include "ECS_components.h"

/*!*****************************************************************************
\brief
The struct for all the dialog's data.
*******************************************************************************/
struct Dialog {
	std::string text = "";
	int speaker = 0;
	int next = 0;
	int next2 = 0;
	int selectedChoice = 0;
};

class DialogManager : public System
{
public:
	/*!*****************************************************************************
	\brief
	Constructor of the DialogManager.
	*******************************************************************************/
	DialogManager();

	/*!*****************************************************************************
	\brief
	Destructor of the DialogManager.
	*******************************************************************************/
	~DialogManager();

	/*!*****************************************************************************
	\brief
	Loads the dialog and it's data into the member map mDialogs.

	\param Dialog _dialog
	The dialog data to be loaded into the map.
	*******************************************************************************/
	void LoadDialog(int _id, Dialog _dialog);

	/*!*****************************************************************************
	\brief
	Prints all the dialog text in the member map mDialogs.
	*******************************************************************************/
	void PrintDialogs();

	/*!*****************************************************************************
	\brief
	Gets the map (mDialogs) filled with all the dialog data.

	\return
	Returns a map of Dialog structs.
	*******************************************************************************/
	std::map<int, Dialog> GetDialogs() { return mDialogs; }

	/*!*****************************************************************************
	\brief
	Checks if the dialog with a specific id has choices.

	\param int _id
	The id of the dialog to be checked.

	\return
	Returns true if the dialog has choices else returns false.
	*******************************************************************************/
	bool HaveChoices(int _id);

	/*!*****************************************************************************
	\brief
	Retrieves the choices of the dialog with a specific id.

	\param int _id
	The id of the dialog to get the choices of.

	\return
	Returns a pair of integers with the choices of the dialog.
	*******************************************************************************/
	std::pair<int, int> GetChoices(int _id);

	/*!*****************************************************************************
	\brief
	Gets the speaker of the dialog with a specific id.

	\param int _id
	The id of the dialog to get the speaker of.

	\return
	Returns the speaker id.
	*******************************************************************************/
	int GetSpeaker(int _id);

	/*!*****************************************************************************
	\brief
	Retrieves the dialog text of the dialog with a specific id.

	\param int _id
	The id of the dialog to get.

	\return
	Returns the dialog text in string.
	*******************************************************************************/
	std::string GetLine(int _id);

	/*!*****************************************************************************
	\brief
	Retrieves the next dialog id.

	\param int _id
	The id of the current dialog.

	\return
	Returns the next dialog id.
	*******************************************************************************/
	int GetNext(int _id);

	/*!*****************************************************************************
	\brief
	Retrieves the second choice dialog id.

	\param int _id
	The id of the current dialog.

	\return
	Returns the second choice dialog id.
	*******************************************************************************/
	int GetNext2(int _id);

	/*!*****************************************************************************
	\brief
	Retrieves the curently selected choice.

	\param int _id
	The id of the current dialog.
	*******************************************************************************/
	int GetSelectedChoice(int _id);

	/*!*****************************************************************************
	\brief
	Set the selected choice.

	\param int _id
	The id of the current dialog.

	\param int _selectedChoice
	The selected choice.
	*******************************************************************************/
	void SetSelectedChoice(int _id, int _selectedChoice);

	/*!*****************************************************************************
	\brief
	Creates a new dialog item from scratch.

	\param int _id
	The id of the new dialog item.

	\param std::string _text
	The text of the new dialog item.

	\param int _speaker
	The speaker id of the new dialog item.

	\param int _next
	The dialog id of the next dialog item/first choice.

	\param int _next2
	The dialog id of the second choice.
	*******************************************************************************/
	void CreateNewDialogue(int _id, std::string _text, int _speaker, int _next, int _next2 = -1);

	/*!*****************************************************************************
	\brief
	Adds a new dialogue.

	\param int _previd
	The id of the previous dialog item.

	\param std::string _text
	The text of the new dialog item.

	\param int _speaker
	The speaker id of the new dialog item.
	*******************************************************************************/
	void AddNewDialogue(int _previd, std::string _text, int _speaker);

	/*!*****************************************************************************
	\brief
	Adds a new dialogue for second option.

	\param int _previd
	The id of the previous dialog item.

	\param std::string _text
	The text of the new dialog item.

	\param int _speaker
	The speaker id of the new dialog item.
	*******************************************************************************/
	void AddNewDialogue2(int _previd, std::string _text, int _speaker);

	/*!*****************************************************************************
	\brief
	Swap with next dialogue item.

	\param int _id
	Id of dialogue item to be swapped.
	*******************************************************************************/
	void SwapNext(int _id);

	/*!*****************************************************************************
	\brief
	Swap with previous dialogue item.

	\param int _id
	Id of dialogue item to be swapped.
	*******************************************************************************/
	void SwapPrev(int _id);

	/*!*****************************************************************************
	\brief
	Swap function.

	\param int _id, _id2, _id3
	Swaps the 3 parameter. 3 > 1 > 2 > 3.
	*******************************************************************************/
	void Swap(int& _id, int& _id2, int& _id3);

	/*!*****************************************************************************
	\brief
	Retrieves the previous dialogue item.

	\param int _id
	Id of current dialogue item.
	*******************************************************************************/
	int GetPrev(int _id);

	/*!*****************************************************************************
	\brief
	Edit a certain dialog from the member map mDialogs.

	\param int _id
	The id of the current dialog.

	\param int _text
	The updated dialog string.
	*******************************************************************************/
	void EditDialogue(int _id, std::string _text);

	/*!*****************************************************************************
	\brief
	Swap the 2 choices options in the member map mDialogs.

	\param int _id
	The id of the dialog choices to be swap.

	\param int _choice
	The first choice.

	\param int _choice2
	The second choice.
	*******************************************************************************/
	void EditChoice(int _id, int _choice, int _choice2);

	/*!*****************************************************************************
	\brief
	Remove a certain dialog from the member map mDialogs.

	\param int _id
	The id of the current dialog.
	*******************************************************************************/
	void RemoveDialog(int _id);

	/*!*****************************************************************************
	\brief
	Get current dialogue id.

	\param int _id
	The id of the current dialog.
	*******************************************************************************/
	bool SetCurrentDialogueID(int _id);

	/*!*****************************************************************************
	\brief
	Get current dialogue id.

	\return
	The id of the current dialog.
	*******************************************************************************/
	int GetCurrentDialogueID();

	/*!*****************************************************************************
	\brief
	Clear the member map mDialogs.
	*******************************************************************************/
	void Clear();

private:
	/*!*****************************************************************************
	\brief
	The map containing all the dialogs' data.
	*******************************************************************************/
	std::map<int, Dialog> mDialogs;

	/*!*****************************************************************************
	\brief
	Current dialogue id.
	*******************************************************************************/
	int mCurrentDialogue;
};