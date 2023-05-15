/*!*****************************************************************************
/*!*****************************************************************************
\file LevelEditor.cpp
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Group: Memory Leak Studios
\date 20-09-2022
\brief
This file contains function declarations for a AssetPanel that view recources
*******************************************************************************/
#include "AssetPanel.h"
#include <ECSManager.h>
#include "Scene.h"
#include "GameState.h"
#include "GameStateManager.h"
#define scrollrate 5
#define minSize 50
#define maxSize 200
/*!*****************************************************************************
\brief
	Initializes the Asset Panel editor

\return
None.
*******************************************************************************/
void AssetPanel::Init()
{
	colNum = 3;
	folderSize = { 95,95 };
	rootPath = "..\\resources";
	sfxIcon = (void*)(intptr_t)spriteManager->GetTextureID("Textures\\Icons\\sfxIcon.png");
	bgmIcon = (void*)(intptr_t)spriteManager->GetTextureID("Textures\\Icons\\bgmIcon.png");
	backIcon = (void*)(intptr_t)spriteManager->GetTextureID("Textures\\Icons\\backIcon.png");
	textIcon = (void*)(intptr_t)spriteManager->GetTextureID("Textures\\Icons\\textIcon.png");
	sceneIcon = (void*)(intptr_t)spriteManager->GetTextureID("Textures\\Icons\\sceneIcon.png");
	scriptIcon = (void*)(intptr_t)spriteManager->GetTextureID("Textures\\Icons\\scriptIcon.png");
	folderIcon = (void*)(intptr_t)spriteManager->GetTextureID("Textures\\Icons\\folderIcon.png");
	prefabIcon = (void*)(intptr_t)spriteManager->GetTextureID("Textures\\Icons\\prefabIcon.png");
	refreshIcon = (void*)(intptr_t)spriteManager->GetTextureID("Textures\\Icons\\refreshIcon.png");
	dialogueIcon = (void*)(intptr_t)spriteManager->GetTextureID("Textures\\Icons\\dialogueIcon.png");
	gamestateIcon = (void*)(intptr_t)spriteManager->GetTextureID("Textures\\Icons\\gamestateIcon.png");
}
/*!*****************************************************************************
\brief
	Update the Asset Panel editor

\return
None.
*******************************************************************************/
void AssetPanel::Update()
{
	if (!active)
		return;
	static bool start = true;
	if (start)
	{
		Init();
		start = false;
	}
	if (ImGui::Begin("Asset Manager"))
	{
		ImGui::BeginTabBar("Assets");
		if (ImGui::BeginTabItem("Content Browser")) {
			// switch to the newly selected tab
			calculateSize();
			static std::filesystem::path m_CurrentDirectory = std::filesystem::path(rootPath);
			if (m_CurrentDirectory != std::filesystem::path(rootPath))
			{
				if (ImGui::ImageButton(backIcon, { folderSize.x, folderSize.y / 2.5f }, ImVec2(0, 1), ImVec2(1, 0)))
				{
					m_CurrentDirectory = m_CurrentDirectory.parent_path();
				}
				ImGui::SameLine(0.f, 1.f);
				ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth()- folderSize.x-25.f, ImGui::GetCursorPosY()));
				if (ImGui::ImageButton(refreshIcon, { folderSize.x, folderSize.y / 2.5f }, ImVec2(0, 1), ImVec2(1, 0)))
				{
					ResourceManager::GetInstance()->LoadAllResources();
				}
			}
			else
			{
				/*ImGui::NewLine();
				ImGui::NewLine();*/
				ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - folderSize.x - 25.f, ImGui::GetCursorPosY()));
				if (ImGui::ImageButton(refreshIcon, { folderSize.x, folderSize.y / 2.5f }, ImVec2(0, 1), ImVec2(1, 0)))
				{
					ResourceManager::GetInstance()->LoadAllResources();
				}
			}
			ImGui::Separator();
			ImGui::NewLine();
			int id = 0;
			for (auto& directory : std::filesystem::directory_iterator(m_CurrentDirectory))
			{
				
				const auto& path = directory.path();
				auto relativePath = std::filesystem::relative(path, rootPath);
				std::string filename = relativePath.filename().string();
				ImGui::Columns(colNum);

				if (directory.is_directory())
				{
					ImGui::ImageButton(folderIcon, folderSize, ImVec2(0, 1), ImVec2(1, 0));
					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						if (directory.is_directory())
							m_CurrentDirectory /= path.filename();
					}
				}
				else
				{
					texPath = directory.path().string();
					texPath = texPath.substr(13);
					texParent = directory.path().parent_path().string();
					texExt = directory.path().extension().string();
					texFilename = directory.path().stem().string();
					if (texExt == ".meta")
						continue;
					ImGui::PushID(id++);
					if (texParent.find("\\Audio\\BGM") != std::string::npos)
					{
						ImGui::ImageButton(bgmIcon, folderSize, ImVec2(0, 1), ImVec2(1, 0));
						if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{
							LOG_INFO("Playing " + texFilename);
							audioManager->PlayAnySound(texFilename, 16);
						}
						if (ImGui::BeginDragDropSource())
						{
							const wchar_t* itemPath = (wchar_t*)texFilename.c_str();
							ImGui::SetDragDropPayload("AUDIO", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
							ImGui::EndDragDropSource();
						}
					}
					else if (texParent.find("\\Audio\\SFX") != std::string::npos)
					{
						ImGui::ImageButton(sfxIcon, folderSize, ImVec2(0, 1), ImVec2(1, 0));
						if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{
							LOG_INFO("Playing " + texFilename);
							audioManager->PlayAnySound(texFilename, 16);
						}
						if (ImGui::BeginDragDropSource())
						{
							const wchar_t* itemPath = (wchar_t*)texFilename.c_str();
							ImGui::SetDragDropPayload("AUDIO", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
							ImGui::EndDragDropSource();
						}
					}
					else if (texParent.find("\\Dialogs") != std::string::npos)
					{
						ImGui::ImageButton(dialogueIcon, folderSize, ImVec2(0, 1), ImVec2(1, 0));
						if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{
							serializationManager->LoadDialogs(texFilename);
							audioManager->LoadDialogueAudio(texFilename);
							SetPannelIsActive(E_PANELID::DIALOGUE, true);
						}
						if (ImGui::BeginDragDropSource())
						{
							const wchar_t* itemPath = (wchar_t*)texFilename.c_str();
							ImGui::SetDragDropPayload("DIALOGUE", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
							ImGui::EndDragDropSource();
						}
					}
					else if (texParent.find("\\Fonts") != std::string::npos)
					{
						ImGui::ImageButton(textIcon, folderSize, ImVec2(0, 1), ImVec2(1, 0));
						if (ImGui::BeginDragDropSource())
						{
							const wchar_t* itemPath = (wchar_t*)texFilename.c_str();
							ImGui::SetDragDropPayload("FONT", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
							ImGui::EndDragDropSource();
						}
					}
					else if (texParent.find("\\GameStates") != std::string::npos)
					{
						ImGui::ImageButton(gamestateIcon, folderSize, ImVec2(0, 1), ImVec2(1, 0));
						if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && texFilename != "GameStateJazz") //REMOVEME
						{
							GameStateManager::GetInstance()->AddGameState(directory.path());
							GameStateManager::GetInstance()->SetGameState(directory.path().stem().string());
							selectedGameState = (int)(*mGameStates).size() - 1;
							selectedScene = (int)(*mGameStates)[selectedGameState].mScenes.size() - 1;
						}
					}
					else if (texParent.find("\\Scene") != std::string::npos)
					{
						ImGui::ImageButton(sceneIcon, folderSize, ImVec2(0, 1), ImVec2(1, 0));
						if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{
							SceneReset();
							if (selectedGameState >= (*mGameStates).size())
							{
								NewGameState();
							}
							GameStateManager::GetInstance()->mCurrentGameState->AddScene(directory.path());
							selectedScene = (int)(*mGameStates)[selectedGameState].mScenes.size() - 1;
						}
					}
					else if (texParent.find("\\Scripts") != std::string::npos)
					{
						ImGui::ImageButton(scriptIcon, folderSize, ImVec2(0, 1), ImVec2(1, 0));
						if (ImGui::BeginDragDropSource())
						{
							const wchar_t* itemPath = (wchar_t*)texFilename.c_str();
							ImGui::SetDragDropPayload("SCRIPT", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
							ImGui::EndDragDropSource();
						}
					}
					else if (texParent.find("\\SCRIPTING") != std::string::npos)
					{
						ImGui::ImageButton(scriptIcon, folderSize, ImVec2(0, 1), ImVec2(1, 0));
						if (ImGui::BeginDragDropSource())
						{
							const wchar_t* itemPath = (wchar_t*)texFilename.c_str();
							ImGui::SetDragDropPayload("SCRIPT", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
							ImGui::EndDragDropSource();
						}
					}
					else if (texParent.find("\\Textures") != std::string::npos)
					{
						selectedIcon = (void*)(intptr_t)spriteManager->GetTextureID(texPath);
						ImGui::ImageButton(selectedIcon, folderSize, ImVec2(0, 1), ImVec2(1, 0));
						if (texExt == ".png" || texExt == ".jpg")
						{
							if (ImGui::BeginDragDropSource())
							{
								const wchar_t* itemPath = (wchar_t*)texPath.c_str();
								ImGui::SetDragDropPayload("TEXTURES", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
								ImGui::EndDragDropSource();
							}
						}
					}
					else if (texParent.find("\\Prefabs") != std::string::npos)
					{
						ImGui::ImageButton(prefabIcon, folderSize, ImVec2(0, 1), ImVec2(1, 0));
						if (ImGui::BeginDragDropSource())
						{
							const wchar_t* itemPath = (wchar_t*)texFilename.c_str();
							ImGui::SetDragDropPayload("PREFAB", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
							ImGui::EndDragDropSource();
						}
						if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{
							serializationManager->LoadPrefab(texFilename);
						}
					}
					else
					{
						ImGui::Button(texFilename.c_str(), folderSize);
					}
					ImGui::PopID();

					/*my_image2_texture = spriteManager->GetTextureID(texPath);
					if (my_image2_texture)
					{
						selectedIcon = (void*)(intptr_t)my_image2_texture;
						ImGui::ImageButton(selectedIcon, buttonSize, ImVec2(0, 1), ImVec2(1, 0));
						if (texExt == ".png" || texExt == ".jpg")
						{
							if (ImGui::BeginDragDropSource())
							{
								const wchar_t* itemPath = (wchar_t*)texPath.c_str();
								ImGui::SetDragDropPayload("TEXTURES", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
								ImGui::EndDragDropSource();
							}
						}

					}
					else
					{
						if(ImGui::Button(filename.c_str(), buttonSize))
						{
							//std::cout << directory.path().parent_path() << "\n";
						}
						if (texExt == ".wav")
						{
							if (ImGui::BeginDragDropSource())
							{
								const wchar_t* itemPath = (wchar_t*)texFilename.c_str();
								ImGui::SetDragDropPayload("AUDIO", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
								ImGui::EndDragDropSource();
							}
						}
						else if (texExt == ".cpp")
						{
							if (ImGui::BeginDragDropSource())
							{
								const wchar_t* itemPath = (wchar_t*)texFilename.c_str();
								ImGui::SetDragDropPayload("SCRIPT", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
								ImGui::EndDragDropSource();
							}
						}
						else if (texExt == ".ttf")
						{
							if (ImGui::BeginDragDropSource())
							{
								const wchar_t* itemPath = (wchar_t*)texFilename.c_str();
								ImGui::SetDragDropPayload("FONT", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
								ImGui::EndDragDropSource();
							}
						}
					}*/
				}
				/*if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if (directory.is_directory())
						m_CurrentDirectory /= path.filename();
					else
					{
						if (texExt == ".json")
						{

							if (texFilename.substr(0, 6) == "Dialog")
							{
								serializationManager->LoadDialogs(texFilename);
								SetPannelIsActive(E_PANELID::DIALOGUE ,true);
							}
							else if (texFilename.substr(0, 5) == "Scene")
							{
								ECS::DestroyAllEntities();
								SceneReset();
								serializationManager->LoadScene(texFilename);
							}
						}
						else if (texExt == ".png")
						{
						}
						else if (texExt == ".wav")
						{
							//audioManager->Play(texfilename);
							std::cout << "play " << texFilename << "\n";
							audioManager->PlayAnySound(texFilename, 16);
						}
					}
				}*/
				ImGui::TextWrapped(filename.c_str());
				ImGui::NextColumn();
			}
			ImGui::Columns(1);
			ImGui::NewLine();
			ImGui::EndTabItem();
		}
		ImGui::Dummy({ 10,100 });
		ImGui::EndTabBar();
	}
	ImGui::End();
}
/*!*****************************************************************************
\brief
	Free the Asset Panel editor

\return
None.
*******************************************************************************/
void AssetPanel::Free()
{

}
void AssetPanel::calculateSize()
{
	if (ImGui::IsWindowHovered() && (Input::CheckKey(E_STATE::HOLD, E_KEY::M_BUTTON_L)) || (Input::CheckKey(E_STATE::HOLD, E_KEY::LEFT_CONTROL)))
	{
		if (Input::GetScroll() > 0.0 && (folderSize.x < maxSize)) //scroll up   // zoon in
		{
			folderSize.x += scrollrate;
			folderSize.y += scrollrate;
		}
		else if (Input::GetScroll() < 0.0 && (folderSize.x > minSize))  //scroll down //zoom out
		{
			folderSize.x -= scrollrate;
			folderSize.y -= scrollrate;
		}
	}
	colNum = (int)(ImGui::GetWindowWidth() / folderSize.x) - 1;
	colNum = colNum > 0 ? colNum : 1;
}