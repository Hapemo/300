/*!*****************************************************************************
\file DialogManager.cpp
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 26-09-2022
\brief
This file contains the function definitions of the class DialogManager.
The DialogManager manages the scripts and dialogues data.
*******************************************************************************/

#include "DialogManager.h"

/*!*****************************************************************************
\brief
Constructor of the DialogManager.
*******************************************************************************/
DialogManager::DialogManager() {
	LOG_CREATE("DIALOG");
}

/*!*****************************************************************************
\brief
Destructor of the DialogManager.
*******************************************************************************/
DialogManager::~DialogManager() {
	Clear();
}

/*!*****************************************************************************
\brief
Loads the dialog and it's data into the member map mDialogs.

\param Dialog _dialog
The dialog data to be loaded into the map.
*******************************************************************************/
void DialogManager::LoadDialog(int _id, Dialog _dialog) {
	mDialogs[_id] = _dialog;
}

/*!*****************************************************************************
\brief
Prints all the dialog text in the member map mDialogs.
*******************************************************************************/
void DialogManager::PrintDialogs() {
	for (std::pair<int, Dialog> dialog : mDialogs)
		LOG_CUSTOM("DIALOG", dialog.second.text.c_str());
}

/*!*****************************************************************************
\brief
Checks if the dialog with a specific id has choices.

\param int _id
The id of the dialog to be checked.

\return
Returns true if the dialog has choices else returns false.
*******************************************************************************/
bool DialogManager::HaveChoices(int _id) {
	if (mDialogs.count(_id)) {
		if (mDialogs[_id].next2) return true;
		else return false;
	} else LOG_ERROR("HaveChoices(" + std::to_string(_id) + "): Dialogue ID doesn't exist!");
	return false;
}

/*!*****************************************************************************
\brief
Gets the speaker of the dialog with a specific id.

\param int _id
The id of the dialog to get the speaker of.

\return
Returns the speaker id.
*******************************************************************************/
int DialogManager::GetSpeaker(int _id) {
	if(mDialogs.count(_id)) return mDialogs[_id].speaker;
	else LOG_ERROR("GetSpeaker(" + std::to_string(_id) + "): Dialogue ID doesn't exist!");
	return -1;
}

/*!*****************************************************************************
\brief
Retrieves the dialog text of the dialog with a specific id.

\param int _id
The id of the dialog to get.

\return
Returns the dialog text in string.
*******************************************************************************/
std::string DialogManager::GetLine(int _id) {
	if (mDialogs.count(_id)) return mDialogs[_id].text;
	else LOG_ERROR("GetDialogue(" + std::to_string(_id) + "): Dialogue ID doesn't exist!");
	return "";
}

/*!*****************************************************************************
\brief
Retrieves the next dialog id.

\param int _id
The id of the current dialog.

\return
Returns the next dialog id.
*******************************************************************************/
int DialogManager::GetNext(int _id) {
	LOG_INFO("Using choice's get next");
	if (mDialogs.count(_id)) return mDialogs[_id].next;
	else LOG_ERROR("GetNext(" + std::to_string(_id) + "): Dialogue ID doesn't exist!");
	return 0;
}

/*!*****************************************************************************
\brief
Retrieves the second choice dialog id.

\param int _id
The id of the current dialog.

\return
Returns the second choice dialog id.
*******************************************************************************/
int DialogManager::GetNext2(int _id) {
	if (mDialogs.count(_id)) return mDialogs[_id].next2;
	else LOG_ERROR("GetNext2(" + std::to_string(_id) + "): Dialogue ID doesn't exist!");
	return 0;
}

/*!*****************************************************************************
\brief
Retrieves the curently selected choice.

\param int _id
The id of the current dialog.
*******************************************************************************/
int DialogManager::GetSelectedChoice(int _id) {
	if (mDialogs.count(_id)) return mDialogs[_id].selectedChoice;
	else LOG_ERROR("GetSelectedChoice(" + std::to_string(_id) + "): Dialogue ID doesn't exist!");
	return 0;
}

/*!*****************************************************************************
\brief
Set the selected choice.

\param int _id
The id of the current dialog.

\param int _selectedChoice
The selected choice.
*******************************************************************************/
void DialogManager::SetSelectedChoice(int _id, int _selectedChoice) {
	if (mDialogs.count(_id)) mDialogs[_id].selectedChoice = _selectedChoice;
	else LOG_ERROR("SetSelectedChoice(" + std::to_string(_id) + "): Dialogue ID doesn't exist!");
}

/*!*****************************************************************************
\brief
Retrieves the choices of the dialog with a specific id.

\param int _id
The id of the dialog to get the choices of.

\return
Returns a pair of integers with the choices of the dialog.
*******************************************************************************/
std::pair<int, int> DialogManager::GetChoices(int _id) {
	std::pair<int, int> choices;
	if (mDialogs.count(_id)) {
		if (!mDialogs[_id].next2) {
			LOG_ERROR("Dialogue doesn't have choices!");
			choices.first = choices.second = 0;
		}
		else {
			choices.first = mDialogs[_id].next;
			choices.second = mDialogs[_id].next2;
		}
	}
	else LOG_ERROR("GetChoices(" + std::to_string(_id) + "): Dialogue ID doesn't exist!");
	return choices;
}

/*!*****************************************************************************
\brief
Edit a certain dialog from the member map mDialogs.

\param int _id
The id of the current dialog.

\param int _text
The updated dialog string.
*******************************************************************************/
void DialogManager::EditDialogue(int _id, std::string _text) {
	if (mDialogs.count(_id))
		mDialogs[_id].text = _text;
}

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
void DialogManager::EditChoice(int _id, int _choice, int _choice2) {
	if (mDialogs.count(_id)) {
		mDialogs[_id].next = _choice;
		mDialogs[_id].next2 = _choice2;
	}
}

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
void DialogManager::CreateNewDialogue(int _id, std::string _text, int _speaker, int _next, int _next2) {
	if (mDialogs.count(_id))
		LOG_ERROR("Dialogue ID already exist!");
	else {
		Dialog newDialog;
		newDialog.text = _text;
		newDialog.speaker = _speaker;
		newDialog.next = _next;
		newDialog.next2 = _next2;
		mDialogs[_id] = newDialog;
	}
}

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
void DialogManager::AddNewDialogue(int _previd, std::string _text, int _speaker) {
	int id = 1;
	for (std::pair<int, Dialog> dialog : mDialogs) {
		if (mDialogs.count(id)) id++;
		else break;
	}
	int next = mDialogs[_previd].next;
	mDialogs[_previd].next = id;
	Dialog newDialog;
	newDialog.text = _text;
	newDialog.speaker = _speaker;
	newDialog.next = next;
	newDialog.next2 = 0;
	newDialog.selectedChoice = 0;
	mDialogs[id] = newDialog;
}

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
void DialogManager::AddNewDialogue2(int _previd, std::string _text, int _speaker) {
	int id = 1;
	for (std::pair<int, Dialog> dialog : mDialogs) {
		if (mDialogs.count(id)) id++;
		else break;
	}
	int next = mDialogs[_previd].next2;
	mDialogs[_previd].next2 = id;
	mDialogs[_previd].selectedChoice = 1;
	Dialog newDialog;
	newDialog.text = _text;
	newDialog.speaker = _speaker;
	newDialog.next = next;
	newDialog.next2 = 0;
	newDialog.selectedChoice = 0;
	mDialogs[id] = newDialog;
}

//int DialogManager::SwapNext(int _id) { // swap text and speaker only
//	int id2 = GetNext(_id);
//	if (id2 == 0)// no more swap down
//		return _id;
//	std::string tmpt = mDialogs[id2].text;
//	mDialogs[id2].text = mDialogs[_id].text;
//	mDialogs[_id].text = tmpt;
//
//	int tmps = mDialogs[id2].speaker;
//	mDialogs[id2].speaker = mDialogs[_id].speaker;
//	mDialogs[_id].speaker = tmps;
//	std::cout << "SWAPPED " << _id << " With " << id2 << "\n";
//	return id2;
//}
//int DialogManager::SwapPrev(int _id) {
//	if (_id == 1)//no more swap up
//		return _id; 
//	int id2 = GetPrev(_id);
//	if (id2 == 0)// no more swap down
//		return _id;
//	std::string tmpt = mDialogs[id2].text;
//	mDialogs[id2].text = mDialogs[_id].text;
//	mDialogs[_id].text = tmpt;
//
//	int tmps = mDialogs[id2].speaker;
//	mDialogs[id2].speaker = mDialogs[_id].speaker;
//	mDialogs[_id].speaker = tmps;
//
//	std::cout << "SWAPPED " << _id << " With " << id2 << "\n";
//	return id2;
//}

/*!*****************************************************************************
\brief
Swap with next dialogue item.

\param int _id
Id of dialogue item to be swapped.
*******************************************************************************/
void DialogManager::SwapNext(int _id) {
	int id3 = GetPrev(_id);
	int id2 = 0;
	if(GetSelectedChoice(_id)) id2 = GetNext2(_id);
	else id2 = GetNext(_id);
	if (id2 == 0 || id3 ==0)// no more swap down
		return;
	Swap(mDialogs[id2].next, mDialogs[_id].next, mDialogs[id3].next);
	Swap(mDialogs[id2].next2, mDialogs[_id].next2, mDialogs[id3].next2);
	Swap(mDialogs[id2].selectedChoice, mDialogs[_id].selectedChoice, mDialogs[id3].selectedChoice);
}

/*!*****************************************************************************
\brief
Swap with previous dialogue item.

\param int _id
Id of dialogue item to be swapped.
*******************************************************************************/
void DialogManager::SwapPrev(int _id) {
	int id2 = GetPrev(_id);
	int id3 = GetPrev(id2);
	if (id2 == 0 || id3 == 0)// no more swap up
		return;
	Swap(mDialogs[_id].next, mDialogs[id2].next, mDialogs[id3].next);
	Swap(mDialogs[_id].next2, mDialogs[id2].next2, mDialogs[id3].next2);
	Swap(mDialogs[_id].selectedChoice, mDialogs[id2].selectedChoice, mDialogs[id3].selectedChoice);
}

/*!*****************************************************************************
\brief
Swap function.

\param int _id, _id2, _id3
Swaps the 3 parameter. 3 > 1 > 2 > 3.
*******************************************************************************/
void DialogManager::Swap(int& _id, int& _id2, int& _id3) {
	int tmp = _id3;
	_id3 = _id2;
	_id2 = _id;
	_id = tmp;
}

/*!*****************************************************************************
\brief
Retrieves the previous dialogue item.

\param int _id
Id of current dialogue item.
*******************************************************************************/
int DialogManager::GetPrev(int _id) {
	for (std::pair<int, Dialog> dialog : mDialogs)
		if (dialog.second.next == _id || dialog.second.next2 == _id)
			return dialog.first;
	return 0;
}

/*!*****************************************************************************
\brief
Clear the member map mDialogs.
*******************************************************************************/
void DialogManager::Clear() {
	mDialogs.clear();
}

/*!*****************************************************************************
\brief
Remove a certain dialog from the member map mDialogs.

\param int _id
The id of the current dialog.
*******************************************************************************/
void DialogManager::RemoveDialog(int _id) {
	if (mDialogs.count(_id)) mDialogs.erase(_id);
	else LOG_ERROR("RemoveDialog(" + std::to_string(_id) + "): Dialogue ID doesn't exist!");
}

/*!*****************************************************************************
\brief
Get current dialogue id.

\param int _id
The id of the current dialog.
*******************************************************************************/
bool DialogManager::SetCurrentDialogueID(int _id) {
	if (mDialogs.count(_id)) {
		mCurrentDialogue = _id;
		return true;
	} else {
		LOG_ERROR("SetCurrentDialogueID(" + std::to_string(_id) + "): Dialogue ID doesn't exist!");
		return false;
	}
}

/*!*****************************************************************************
\brief
Get current dialogue id.

\return
The id of the current dialog.
*******************************************************************************/
int DialogManager::GetCurrentDialogueID() {
	return mCurrentDialogue;
}
