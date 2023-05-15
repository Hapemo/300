/*!*****************************************************************************
/*!*****************************************************************************
\file DialoguePanel.cpp
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Group: Memory Leak Studios
\date 20-10-2022
\brief
This file contains function definations for a Dialogue Panel Editor system that modifies
and view dialogues between a player and npc.
*******************************************************************************/
#include "DialoguePanel.h"
#include <ECSManager.h>
#define iconSize ImVec2(40, 40)
#define sendbuttonSize ImVec2(55, 55)
/*!*****************************************************************************
\brief
	Initializes the Dialogue Panel editor

\return
None.
*******************************************************************************/
void DialoguePanel::Init()
{
	//To be loaded from config file in the future...
	playerIcon = (void*)(intptr_t)spriteManager->GetTextureID("Textures\\Sprites\\mc.png");
	npcIcon = (void*)(intptr_t)spriteManager->GetTextureID("Textures\\Sprites\\girl.png");
	sendIcon = (void*)(intptr_t)spriteManager->GetTextureID("Textures\\Icons\\sendIcon.png");
	selectedID = 0;
	unselectedCol= ImVec4(ImColor(220, 30, 58));
	selectedCol= ImVec4(ImColor(0, 128, 0));
	npcCol= ImVec4(ImColor(0, 136, 204));
	backgroundCol=ImVec4(ImColor(50, 50, 50));
}
/*!*****************************************************************************
\brief
	Updates the Dialogue Panel editor

\return
None.
*******************************************************************************/
void DialoguePanel::Update()
{
	static bool scrollToBottom = false;
	
	if (ImGui::Begin("Dialog Editor"))
	{
		if (scrollToBottom)
		{
			ImGui::SetScrollY(ImGui::GetWindowHeight() * 10.f);
			scrollToBottom = false;
		}
		renderUI();
		if (!isViewportPaused)
		{
			NarrateDialogue();
		}
		wrapsize = int(ImGui::GetWindowWidth() / 13);
		int id = 1;
		int id2 = 0;
		int prevID = 0;
		int prevPlayerID = 0;
		int prevNpcID = 0;

		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - sendbuttonSize.x - 30);
		ImGui::PushStyleColor(ImGuiCol_Button, unselectedCol);
		if (ImGui::Button("X", sendbuttonSize))
		{
			dialogManager->Clear();
		}
		ImGui::PopStyleColor();

		while (id)
		{
			if (dialogManager->GetDialogs().size() == 0)
			{
				//active = false;
				break;
			}
			dialog = dialogManager->GetLine(id);
			//if (id == 1 && dialog != dialogPrev) //reset selected if change dialogue
			//{
			//	selectedID = 0;
			//	prevSelectedID = 0;
			//	dialogPrev = dialog;
			//}
			BreakString(dialog, wrapsize);

			if (dialogManager->GetSpeaker(id))// if right side convo (Player)
			{
				ImGui::NewLine();
				ImGui::SameLine(ImGui::GetWindowWidth() - 70);
				static bool c = false;
				ImGui::ImageButton(playerIcon, iconSize, ImVec2(0, 1), ImVec2(1, 0));

				ImGui::SameLine(ImGui::GetWindowWidth() - 77 - (dialog.find("\n") != std::string::npos ? dialog.find("\n") : dialog.size()) * 7.4f);
				if (prevID && dialogManager->GetSelectedChoice(prevID))//2nd choice selected
					ImGui::PushStyleColor(ImGuiCol_Button, unselectedCol); //unselected
				else//1st
					ImGui::PushStyleColor(ImGuiCol_Button, selectedCol); //selected
			}
			else //left side (NPC)
			{
				ImGui::ImageButton(npcIcon, iconSize, ImVec2(0, 1), ImVec2(1, 0));
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Button, npcCol); //passenger
			}
			ImGui::Button(dialog.c_str());
			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
			{//edit text for 1st choice
				selectedID = id;
				dialogEdit = dialogManager->GetLine(selectedID);
				BreakString(dialogEdit, wrapsize);
			}
			else if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
			{//change selected to 1st choice
				if (prevID)
					dialogManager->SetSelectedChoice(prevID, 0);
			}
			else if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1))
			{//play sound
				isViewportPaused = true;
				audioManager->PlayDialogueSound(std::to_string(id), 17);
			}
			if (id2) //if have 2nd choice
			{
				dialog2 = dialogManager->GetLine(id2);
				BreakString(dialog2, wrapsize);
				if (dialogManager->GetSpeaker(id2))
				{
					ImGui::NewLine();
					ImGui::SameLine(ImGui::GetWindowWidth() - 70);
					ImGui::ImageButton(playerIcon, iconSize, ImVec2(0, 1), ImVec2(1, 0));
					ImGui::SameLine(ImGui::GetWindowWidth() - 77 - (dialog2.find("\n") != std::string::npos ? dialog2.find("\n") : dialog2.size()) * 7.4f);
					if (prevID && dialogManager->GetSelectedChoice(prevID))//2nd choice selected
						ImGui::PushStyleColor(ImGuiCol_Button, selectedCol); //selected
					else//1st
						ImGui::PushStyleColor(ImGuiCol_Button, unselectedCol); //unselected
				}
				else //left side (NPC)
				{
					ImGui::PushStyleColor(ImGuiCol_Button, npcCol);
				}
				ImGui::Button(dialog2.c_str());
				ImGui::PopStyleColor();
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
				{//edit text for 2nd choice
					selectedID = id2;
					dialogEdit = dialogManager->GetLine(selectedID);
					BreakString(dialogEdit, wrapsize);
				}
				else if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
				{//change selected to 2nd choice
					dialogManager->SetSelectedChoice(prevID, 1);
				}
				else if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1))
				{//play sound
					isViewportPaused = true;
					audioManager->PlayDialogueSound(std::to_string(id2), 17);
				}
			}
			//Get new ID for next loop
			if (prevID && dialogManager->GetSelectedChoice(prevID))//2nd
				prevID = id2;
			else//1st
				prevID = id;
			id = dialogManager->GetNext(prevID);
			id2 = dialogManager->GetNext2(prevID);
			if (dialogManager->GetSpeaker(prevID))
			{
				prevPlayerID = prevID;
			}
			else
			{
				prevNpcID = prevID;
			}
			ImGui::NewLine();
		}
		ImGui::Separator();
		ImGui::Dummy({ 10,100 });
		ImGui::PushStyleColor(ImGuiCol_ChildBg, backgroundCol);
		ImGui::SetCursorPos(ImVec2(10, ImGui::GetWindowHeight() - 90 + ImGui::GetScrollY()));
		ImGui::BeginChild(" ", ImVec2(ImGui::GetWindowWidth() - 20, 80), true);
		if (selectedID) //edit text
		{
			ImGui::PushID(1);
			ImGui::InputTextMultiline("", &dialogEdit, ImVec2(ImGui::GetWindowWidth() - 90, sendbuttonSize.y));
			ImGui::PopID();
			ImGui::SameLine(0.0f, 5.f);
			if (ImGui::ImageButton(sendIcon, sendbuttonSize, ImVec2(0, 1), ImVec2(1, 0)))
			{
				dialogManager->EditDialogue(selectedID, FormatString(dialogEdit));
				selectedID = 0;
				dialogEdit = "";
			}
			if (Input::CheckKey(E_STATE::PRESS, E_KEY::DOWN))
			{
				dialogManager->SwapNext(selectedID);
			}
			if (Input::CheckKey(E_STATE::PRESS, E_KEY::UP))
			{
				dialogManager->SwapPrev(selectedID);
			}
		}
		else //new text
		{
			if (ImGui::ImageButton(npcIcon, sendbuttonSize, ImVec2(0, 1), ImVec2(1, 0), 1))
			{
				if (dialogEdit != "")
				{
					dialogManager->AddNewDialogue(prevID, dialogEdit, 0);
					dialogEdit = "";
					scrollToBottom = true;
				}
			}
			ImGui::SameLine(0.0f, 8.f);
			ImGui::PushID(2);
			ImGui::InputTextMultiline("", &dialogEdit, ImVec2(ImGui::GetWindowWidth() - 155, sendbuttonSize.y));
			ImGui::PopID();
			ImGui::SameLine(0.0f, 8.f);
			if (prevID != prevPlayerID)
			{
				if (ImGui::ImageButton(playerIcon, sendbuttonSize, ImVec2(0, 1), ImVec2(1, 0), 1, selectedCol))
				{
					if (dialogEdit != "")
					{
						dialogManager->AddNewDialogue(prevID, dialogEdit, 1);
						dialogEdit = "";
						scrollToBottom = true;
					}
				}
			}
			else
			{
				ImVec2 pos = ImGui::GetCursorPos();
				ImGui::PushID(4);
				if (ImGui::ImageButton(playerIcon, { sendbuttonSize.x, sendbuttonSize.y / 2.f }, ImVec2(0, 1), ImVec2(1, 0.5f), 1, selectedCol))
				{
					if (dialogEdit != "")
					{
						dialogManager->AddNewDialogue(prevID, dialogEdit, 1);
						dialogEdit = "";
						scrollToBottom = true;
					}
				}
				ImGui::PopID();
				ImGui::SetCursorPos({ pos.x, pos.y + sendbuttonSize.y / 2.f });
				ImGui::PushID(5);
				if (ImGui::ImageButton(playerIcon, { sendbuttonSize.x, sendbuttonSize.y / 2.f }, ImVec2(0, 0.5f), ImVec2(1, 0), 1, unselectedCol))
				{
					if (dialogEdit != "")
					{
						dialogManager->AddNewDialogue2(prevNpcID, dialogEdit, 1);
						dialogEdit = "";
						scrollToBottom = true;
					}
				}
				ImGui::PopID();
			}
		}
		ImGui::EndChild();
		ImGui::PopStyleColor();
	}
	ImGui::End();
}
/*!*****************************************************************************
\brief
	Free the Dialogue Panel editor

\return
None.
*******************************************************************************/
void DialoguePanel::Free()
{

}


void DialoguePanel::NarrateDialogue()
{
	if (dialogManager->GetDialogs().size() == 0)
	{
		return;
	}
	static int id = 1;
	static int id2 = 0;
	static int prevID = 1;

		
	if (!audioManager->isPlaying(17))
	{
		if (dialogManager->GetSelectedChoice(prevID))
		{
			audioManager->PlayDialogueSound(std::to_string(id2), 17);
			prevID = id2;
		}
		else
		{
			audioManager->PlayDialogueSound(std::to_string(id), 17);
			prevID = id;
		}
		id = dialogManager->GetNext(prevID);
		id2 = dialogManager->GetNext2(prevID);
	}

	if (id == 0)
	{
		isViewportPaused = true;
		id = 1;
		id2 = 0;
		prevID = 1;
	}
}



/*!*****************************************************************************
\brief
	This function removes duplicate white spaces and new line characters in a string
\param _str
	string to be formated
\return
	formated string
*******************************************************************************/
std::string DialoguePanel::FormatString(std::string _str)
{
	std::string editedStr;
	std::string word;
	std::istringstream ss(_str);

	while (ss >> word) {
		if (!editedStr.empty()) {
			editedStr += ' ';
		}
		editedStr += word;
	}
	return editedStr;
}
/*!*****************************************************************************
\brief
	This function breaks a string by adding a string or character at specific intervals
	for text wraping
\param _str
	string to be formated
\param _offset
	minimun off set to break at
\param _breakAt
	character to break at 
	default = ' ' 
\param _breakWith
	string to break _str with
	default = "\n"
\return
	formated string
*******************************************************************************/
std::string& DialoguePanel::BreakString(std::string& _str, int _offset, char _breakAt, std::string _breakWith)
{
	int offset = 0;
	while (_str.size() > _offset)
	{
		if (_str[_offset] == _breakAt)
		{
			_str.replace(_offset, 1, _breakWith);
			_offset += 50;
		}
		else
		{
			offset = (int)_str.find(_breakWith, _offset);
			_offset += offset;
		}
	}
	return _str;
}

