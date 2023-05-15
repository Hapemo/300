/*!*****************************************************************************
\file MenuPanel.cpp
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Group: Memory Leak Studios
\date 20-10-2022
\brief
	This class encapsulates the functions for the Level Editor Main menu
*******************************************************************************/
#include "MenuPanel.h"
#include <ECSManager.h>
#include "GameStateManager.h"
/*!*****************************************************************************
\brief
	Initializes the Menu Panel editor

\return
None.
*******************************************************************************/
void MenuPanel::Init()
{
	
}
/*!*****************************************************************************
\brief
	Updates the Menu Panel editor

\return
None.
*******************************************************************************/
void MenuPanel::Update()
{
	static char filenameS_GameState[30] = "";
	static char filenameO_GameState[30] = "";
	static char filenameS_Scene[30] = "";
	static char filenameO_Scene[30] = "";
	static char filenameS_Dialog[30] = "";
	static char filenameO_Dialog[30] = "";
	static float Disco = 0;
	static float DiscoRate = 0.04f;
	if (Disco != 0)
	{
		Disco += DiscoRate;
		if ((int)Disco % 3 == 0)
			ImGui::StyleColorsDark();
		if ((int)Disco % 3 == 1)
			ImGui::StyleColorsClassic();
		if ((int)Disco % 3 == 2)
			ImGui::StyleColorsLight();
		if (Disco > 100.f)
			Disco = 1.f;
	}
	if (ImGui::BeginMainMenuBar())
	{
		if (Input::CheckKey(E_STATE::HOLD, E_KEY::LEFT_CONTROL) && Input::CheckKey(E_STATE::PRESS, E_KEY::Z))//relocate
		{
			Undo();
		}
		if (Input::CheckKey(E_STATE::HOLD, E_KEY::LEFT_CONTROL) && Input::CheckKey(E_STATE::PRESS, E_KEY::Y))//relocate
		{
			Redo();
		}
		if (Input::CheckKey(E_STATE::HOLD, E_KEY::LEFT_CONTROL) && Input::CheckKey(E_STATE::HOLD, E_KEY::LEFT_SHIFT) && Input::CheckKey(E_STATE::PRESS, E_KEY::Z))//relocate
		{
			Redo();
		}
		if (Input::CheckKey(E_STATE::HOLD, E_KEY::LEFT_CONTROL) && Input::CheckKey(E_STATE::PRESS, E_KEY::C))//relocate
		{
			Copy();
		}
		if (Input::CheckKey(E_STATE::HOLD, E_KEY::LEFT_CONTROL) && Input::CheckKey(E_STATE::PRESS, E_KEY::V))//relocate
		{
			Paste();
		}
		if (Input::CheckKey(E_STATE::HOLD, E_KEY::LEFT_CONTROL) && Input::CheckKey(E_STATE::PRESS, E_KEY::X))//relocate
		{
			Cut();
		}
		if (ImGui::BeginMenu("File"))
		{
			int id = 0;
			ImGui::MenuItem("(menu)", NULL, false, false);

			//gamestate
			ImGui::MenuItem("Open GameState File", NULL, false, false);
			ImGui::PushID(id++);
			ImGui::InputText(".json", filenameO_GameState, 30);
			ImGui::PopID();
			if (ImGui::MenuItem("Open GameState", "Ctrl+O"))
			{
				std::string name = "../resources/GameStates/";
				name += filenameO_GameState;
				name += ".json";
				std::filesystem::path path{ name };
				GameStateManager::GetInstance()->AddGameState(path);
				GameStateManager::GetInstance()->SetGameState(path.stem().string());
				selectedGameState = (int)(*mGameStates).size() - 1;
				selectedScene = (int)(*mGameStates)[selectedGameState].mScenes.size() - 1;
				
			}
			ImGui::Separator();
			ImGui::MenuItem("Open GameState File", NULL, false, false);
			ImGui::PushID(id++);
			ImGui::InputText(".json", filenameS_GameState, 30);
			ImGui::PopID();
			if (ImGui::MenuItem("Save GameState As", "Ctrl+S"))
			{
				(*mGameStates)[selectedGameState].Save();
			}


			//scene
			ImGui::MenuItem("Open Scene File", NULL, false, false);
			ImGui::PushID(id++);
			ImGui::InputText(".json", filenameO_Scene, 30);
			ImGui::PopID();
			if (ImGui::MenuItem("Open Scene", "Ctrl+O"))
			{
				if (selectedGameState >= (*mGameStates).size())
				{
					NewGameState();
				}
				std::string name = "../resources/Scene/";
				name += filenameO_Scene;
				name += ".json";
				std::filesystem::path path{ name };
				GameStateManager::GetInstance()->mCurrentGameState->AddScene(path);
				selectedScene = (int)(*mGameStates)[selectedGameState].mScenes.size() - 1;
			}
			ImGui::Separator();
			ImGui::MenuItem("Open Scene File", NULL, false, false);
			ImGui::PushID(id++);
			ImGui::InputText(".json", filenameS_Scene, 30);
			ImGui::PopID();
			if (ImGui::MenuItem("Save Scene As", "Ctrl+S"))
			{
				(*mGameStates)[selectedGameState].mScenes[selectedScene].Save();
			}


			ImGui::Separator();
			if (ImGui::MenuItem("Clear Scene"))
			{
				ECS::DestroyAllEntities();
				SceneReset();
			}
			ImGui::Separator();

			//dialogue
			ImGui::MenuItem("Open Dialogue File", NULL, false, false);
			ImGui::PushID(id++);
			ImGui::InputText(".json", filenameO_Dialog, 20);
			ImGui::PopID();
			if (ImGui::MenuItem("Open Dialog", "Ctrl+D"))
			{
				serializationManager->LoadDialogs(filenameO_Dialog);
				SetPannelIsActive(E_PANELID::DIALOGUE, true);
			}
			ImGui::Separator();
			ImGui::MenuItem("Save Dialogue File As", NULL, false, false);
			ImGui::PushID(id++);
			ImGui::InputText(".json", filenameS_Dialog, 20);
			ImGui::PopID();
			if (ImGui::MenuItem("Save Dialog As", "Ctrl+F"))
			{
				serializationManager->SaveDialogs(filenameS_Dialog);
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Print Dialogs"))
			{
				dialogManager->PrintDialogs();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Cut", "CTRL+X")) 
			{
				Cut();
			}
			if (ImGui::MenuItem("Copy", "CTRL+C")) 
			{
				Copy();
			}
			if (ImGui::MenuItem("Paste", "CTRL+V")) 
			{
				Paste();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Undo", "CTRL+Z"))
			{
				Undo();
			}
			if (ImGui::MenuItem("Redo", "CTRL+Y"))
			{
				Redo();
			}
			ImGui::EndMenu();

		}
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::BeginMenu("Change Theme"))
			{
				
				if (ImGui::MenuItem("Dark"))
				{
					Disco = 0;
					ImGui::StyleColorsDark();
				}
				if (ImGui::MenuItem("Light"))
				{
					Disco = 0;
					ImGui::StyleColorsLight();
				}
				if (ImGui::MenuItem("Classic"))
				{
					Disco = 0;
					ImGui::StyleColorsClassic();
				}
				if (ImGui::MenuItem("Disco"))
				{
					Disco = 1;
				}
				if (Disco != 0 && ImGui::BeginMenu("Disco Rate"))
				{
					ImGui::DragFloat("Rate", &DiscoRate, 0.005f, 0.01f, 0.7f);
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Change Song Lol :)"))
			{
				for (std::string song : audioManager->GetSongs())
				{
					if (ImGui::MenuItem(song.c_str())) {
						audioManager->PlayBGSound(song, (int)E_AUDIO_CHANNEL::EDITORSONG);
					}
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Stop Song :("))
			{
				audioManager->StopSound((int)E_AUDIO_CHANNEL::EDITORSONG);
			}
			if (!GetPannelIsActive(E_PANELID::DEBUG) && ImGui::MenuItem("Show Debug Info"))
			{
				SetPannelIsActive(E_PANELID::DEBUG, true);
			}
			if (GetPannelIsActive(E_PANELID::DEBUG) && ImGui::MenuItem("Hide Debug Info"))
			{
				SetPannelIsActive(E_PANELID::DEBUG, false);
			}
			/*if (ImGui::MenuItem("Reset panels"))
			{
				ImGuiIO& io = ImGui::GetIO();
				io.IniFilename = "imguiReset.ini";
			}*/
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}
/*!*****************************************************************************
\brief
	Free the Menu Panel editor

\return
None.
*******************************************************************************/
void MenuPanel::Free()
{
}