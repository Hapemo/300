/*!*************************************************************************
****
\file ContentBrowser.cpp
\author Han ChengWen, Kew Yu Jun, Charissa Yip, Jazz Teoh Yu Jue
\par DP email: c.han@digipen.edu, charissa.yip@digipen.edu
\date 28-09-2023
\brief
This file contains ContentBrowser window functions that display
the relative folder and files to a directory stated, (currently hardcoded)
allows user to broswse through the files and drag (.png) files into texture
component (must open) to change texture of object during runtime.

The functions
- update
Contains main loop for the logic of contentbrowsing + Drag and drop functionality


****************************************************************************
***/


#include "ContentBrower.h"
#include "PrefabWindow.h"
#include "ResourceManagerTy.h"
#include "GameState/GameStateManager.h"
#include "Misc.h"
#include <stdlib.h>
#include <iostream>
#include <string>
#include "imgui_stdlib.h"


static const std::filesystem::path s_asset_directory = "../assets";
bool ContentBrowser::dragCheck = false;

ContentBrowser::ContentBrowser():current_Directory(s_asset_directory) {} // constructor to set base directory

/***************************************************************************/
/*!
\brief
	Helper function for checking file extension
*/
/**************************************************************************/
bool check_extension(std::string file, std::string extension) {
	size_t path_length = file.length();
	std::string path_extension = file.substr(path_length - extension.length());
	if (path_extension == extension)
		return true;

	return false;
}
/***************************************************************************/
/*!
\brief
	Helper function to format file path from "\\" to "/"
*/
/**************************************************************************/
void format_string(std::string& path) {

	std::replace(path.begin(), path.end(), '\\', '/');
}
/***************************************************************************/
/*!
\brief
	Update function for ImguiWindow
*/
/**************************************************************************/
void ContentBrowser::update() 
{
	ImGui::Checkbox("Non-Icon", &content2);
	
	ImGui::Text("Filter: ");
	ImGui::SameLine();
	ImGui::InputText("##Filter", &mFilter);

	std::transform(mFilter.begin(), mFilter.end(), mFilter.begin(), toupper);




	if (content2 == true) {
		ImVec2 maxpos;

		if (current_Directory != std::filesystem::path(s_asset_directory))
		{
			if (ImGui::Button("<<", ImVec2(40, 40)))
			{
				current_Directory = current_Directory.parent_path();
			}
		}

		static float padding{ 10 };
		static float buttonsize{ 100 };
		float cellsize = buttonsize + padding;

		float panelwidth = ImGui::GetContentRegionAvail().x;
		int columncount = (int)(panelwidth / cellsize);
		if (columncount < 1) {
			columncount = 1;
		}


		ImGui::Columns(columncount, 0, false);

		auto& resourceDatas = systemManager->mResourceTySystem;
		int idd{ 0 };

		// looping through filesystem
		for (auto const& directory : std::filesystem::directory_iterator{ current_Directory }) {
			const auto& path = directory.path(); // directory path


			std::string filename_string = path.filename().string();


			std::string fsupper = filename_string;
			std::transform(fsupper.begin(), fsupper.end(), fsupper.begin(),toupper);

			
			if (fsupper.length()&& fsupper.find(mFilter) == std::string::npos)
				continue;


			//char to_upper(const char& ch) {
			//	return std::toupper(ch);
			//}

			if (directory.is_directory()) {
				

					ImGui::PushID(idd);
					ImGui::ImageButton((ImTextureID)(intptr_t)resourceDatas->m_EditorTextures["Folder"]->ID(), { buttonsize, buttonsize });
					ImGui::PopID();
					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
						current_Directory /= path.filename();
					}
					ImGui::Text(filename_string.c_str());
					ImGui::NextColumn();


			}
			else if (!directory.is_directory()) {

				if (check_extension(path.string(), ".prefab")) {
					ImGui::PushID(idd);
					ImGui::ImageButton((ImTextureID)(intptr_t)resourceDatas->m_EditorTextures["3DFileIcon"]->ID(), { buttonsize, buttonsize });
					ImGui::PopID();
					if (ImGui::BeginDragDropSource()) {

						std::string path_str = path.string();


						int posstart = static_cast<int>(path_str.find_last_of("\\"));
						int posend = static_cast<int>(path_str.find_last_of("."));

						std::string newpath = path_str.substr(posstart + 1, posend - (posstart + 1));

						const char* source_path = newpath.c_str();
						ImGui::SetDragDropPayload("FILE_PREFAB", source_path, strlen(source_path) * sizeof(wchar_t), ImGuiCond_Once);

						ImGui::EndDragDropSource();
					}

					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {


						int posstart = static_cast<int>(filename_string.find_last_of("\\"));
						int posend = static_cast<int>(filename_string.find_last_of("."));
						std::string newpath = filename_string.substr(posstart + 1, posend);

						Entity toEdit = systemManager->ecs->StartEditPrefab(newpath);
						PrefabWindow::prefabObj = toEdit.id;

					}
					ImGui::Text(filename_string.c_str());
					ImGui::NextColumn();

				}
				else if (check_extension(path.string(), ".gs")) {

					ImGui::ImageButton((ImTextureID)(intptr_t)resourceDatas->m_EditorTextures["3DFileIcon"]->ID(), { buttonsize, buttonsize });
					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
						systemManager->mGameStateSystem->GetCurrentGameState()->Save();
						systemManager->mGameStateSystem->ChangeGameState(Misc::GetFileName(path.string()));
					}
					ImGui::Text(filename_string.c_str());
					ImGui::NextColumn();

				}

				else if (check_extension(path.string(), ".lua")) {
					ImGui::PushID(idd);
					ImGui::ImageButton((ImTextureID)(intptr_t)resourceDatas->m_EditorTextures["UnknownIcon"]->ID(), { buttonsize, buttonsize });
					ImGui::PopID();
					if (ImGui::BeginDragDropSource()) {

						std::string path_str = path.string();

						format_string(path_str);

						const char* source_path = path_str.c_str();
						ImGui::SetDragDropPayload("FILE_LUA", source_path, strlen(source_path) * sizeof(wchar_t), ImGuiCond_Once);

						ImGui::EndDragDropSource();
					}
					ImGui::Text(filename_string.c_str());
					ImGui::NextColumn();

				}


				else if (check_extension(path.string(), ".geom")) {
					ImGui::PushID(idd);
					ImGui::ImageButton((ImTextureID)(intptr_t)resourceDatas->m_EditorTextures["PhysicsMaterial"]->ID(), { buttonsize, buttonsize });
					ImGui::PopID();
					if (ImGui::BeginDragDropSource()) {

						std::string path_str = path.string();

						//format the string from \\ to /.
						format_string(path_str);
						const char* source_path = path_str.c_str();
						ImGui::SetDragDropPayload("FILE_GEOM", source_path, strlen(source_path) * sizeof(wchar_t), ImGuiCond_Once);

						ImGui::EndDragDropSource();
					}
					ImGui::Text(filename_string.c_str());
					ImGui::NextColumn();

				}
				else if (check_extension(path.string(), ".fbx")) {
					ImGui::PushID(idd);
					ImGui::ImageButton((ImTextureID)(intptr_t)resourceDatas->m_EditorTextures["PhysicsMaterial"]->ID(), { buttonsize, buttonsize });
					ImGui::PopID();
					if (ImGui::BeginDragDropSource()) {

						std::string path_str = path.string();

						//format the string from \\ to /.
						format_string(path_str);
						const char* source_path = path_str.c_str();
						ImGui::SetDragDropPayload("FILE_FBX", source_path, strlen(source_path) * sizeof(wchar_t), ImGuiCond_Once);

						ImGui::EndDragDropSource();
					}
					ImGui::Text(filename_string.c_str());
					ImGui::NextColumn();

				}
				else if (check_extension(path.string(), ".ctexture")) {
					ImGui::PushID(idd);
					ImGui::ImageButton((ImTextureID)(intptr_t)resourceDatas->m_EditorTextures["ImageIcon"]->ID(), { buttonsize, buttonsize });
					ImGui::PopID();
					if (ImGui::BeginDragDropSource()) {

						std::string path_str = path.string();

						//format the string from \\ to /.
						format_string(path_str);
						const char* source_path = path_str.c_str();
						ImGui::SetDragDropPayload("FILE_TEXT", source_path, strlen(source_path) * sizeof(wchar_t), ImGuiCond_Once);

						ImGui::EndDragDropSource();
					}
					ImGui::Text(filename_string.c_str());
					ImGui::NextColumn();

				}
				else if (check_extension(path.string(), ".png") || check_extension(path.string(), ".jpg") || check_extension(path.string(), ".jpeg")) {
					ImGui::PushID(idd);
					ImGui::ImageButton((ImTextureID)(intptr_t)resourceDatas->m_EditorTextures["ImageIcon"]->ID(), { buttonsize, buttonsize });
					ImGui::PopID();
					if (ImGui::BeginDragDropSource()) {

						std::string path_str = path.string();

						//format the string from \\ to /.
						format_string(path_str);
						const char* source_path = path_str.c_str();
						ImGui::SetDragDropPayload("FILE_TEXT_UNCOMPRESS", source_path, strlen(source_path) * sizeof(wchar_t), ImGuiCond_Once);

						ImGui::EndDragDropSource();
					}
					ImGui::Text(filename_string.c_str());
					ImGui::NextColumn();

				}
				else if (check_extension(path.string(), ".scn")) {
					ImGui::PushID(idd);
					ImGui::ImageButton((ImTextureID)(intptr_t)resourceDatas->m_EditorTextures["Electro"]->ID(), { buttonsize, buttonsize });
					ImGui::PopID();
					if (ImGui::BeginDragDropSource()) {

						std::string path_str = path.string();

						//format the string from \\ to /.
						format_string(path_str);
						const char* source_path = path_str.c_str();
						ImGui::SetDragDropPayload("FILE_SCN", source_path, strlen(source_path) * sizeof(wchar_t), ImGuiCond_Once);

						ImGui::EndDragDropSource();
					}
					ImGui::Text(filename_string.c_str());
					ImGui::NextColumn();

				}

				else if (check_extension(path.string(), ".wav")) {

					ImGui::PushID(idd);
					ImGui::ImageButton((ImTextureID)(intptr_t)resourceDatas->m_EditorTextures["Electro"]->ID(), { buttonsize, buttonsize });
					ImGui::PopID();
					if (ImGui::BeginDragDropSource()) {

						std::string path_str = path.string();

						int posstart = (int)path_str.find_last_of("\\");
						int posend = (int)path_str.find_last_of(".");

						std::string newpath = path_str.substr(posstart + 1, posend - (posstart + 1));

						//const char* audio_file_name = newpath.c_str();

						ImGui::SetDragDropPayload("FILE_AUDIO", path_str.c_str(), strlen(path_str.c_str()) * sizeof(wchar_t), ImGuiCond_Once);

						ImGui::EndDragDropSource();
					}
					ImGui::Text(filename_string.c_str());
					ImGui::NextColumn();
				}


				else if (check_extension(path.string(), ".mp3")) {

					ImGui::PushID(idd);
					ImGui::ImageButton((ImTextureID)(intptr_t)resourceDatas->m_EditorTextures["Electro"]->ID(), { buttonsize, buttonsize });
					ImGui::PopID();
					if (ImGui::BeginDragDropSource()) {

						std::string path_str = path.string();

						int posstart = (int)path_str.find_last_of("\\");
						int posend = (int)path_str.find_last_of(".");

						std::string newpath = path_str.substr(posstart + 1, posend - (posstart + 1));

						//const char* audio_file_name = newpath.c_str();

						ImGui::SetDragDropPayload("FILE_AUDIO", path_str.c_str(), strlen(path_str.c_str()) * sizeof(wchar_t), ImGuiCond_Once);

						ImGui::EndDragDropSource();
					}
					ImGui::Text(filename_string.c_str());
					ImGui::NextColumn();
				}
			}

			idd++;


		}
		ImGui::Columns(1);
	}
	else {
	static std::filesystem::path directoryBrowser = s_asset_directory;

	if (ImGui::BeginTable("Table", 2, ImGuiTableFlags_Resizable))
	{
		ImGui::TableNextRow();

		//left menu
		ImGui::TableSetColumnIndex(0);

		for (auto const& directory : std::filesystem::directory_iterator{ directoryBrowser }) {
			const auto& path = directory.path(); // directory path
			auto relativepath = std::filesystem::relative(path, directoryBrowser);
			std::string filename_string = relativepath.filename().string();

			if (directory.is_directory()) {
				ImGui::Selectable(filename_string.c_str());
				if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
					current_Directory = directoryBrowser;
					current_Directory /= path.filename();
				}
			}

		}

		ImGui::TableSetColumnIndex(1);
		ImVec2 maxpos;


		static float padding{ 10 };
		static float buttonsize{ 60 };
		float cellsize = buttonsize + padding;

		float panelwidth = ImGui::GetContentRegionAvail().x;
		int columncount = (int)(panelwidth / cellsize);
		if (columncount < 1) {
			columncount = 1;
		}



		auto& resourceDatas = systemManager->mResourceTySystem;
		int idd{ 0 };

		// looping through filesystem
		for (auto const& directory : std::filesystem::directory_iterator{ current_Directory }) {
			const auto& path = directory.path(); // directory path

			std::string filename_string = path.filename().string();

			if (!directory.is_directory()) {

				if (check_extension(path.string(), ".prefab")) {
					ImGui::Selectable(filename_string.c_str());
					if (ImGui::BeginDragDropSource()) {

						std::string path_str = path.string();


						int posstart = static_cast<int>(path_str.find_last_of("\\"));
						int posend = static_cast<int>(path_str.find_last_of("."));

						std::string newpath = path_str.substr(posstart + 1, posend - (posstart + 1));

						const char* source_path = newpath.c_str();
						ImGui::SetDragDropPayload("FILE_PREFAB", source_path, strlen(source_path) * sizeof(wchar_t), ImGuiCond_Once);

						ImGui::EndDragDropSource();
					}

					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {


						int posstart = static_cast<int>(filename_string.find_last_of("\\"));
						int posend = static_cast<int>(filename_string.find_last_of("."));
						std::string newpath = filename_string.substr(posstart + 1, posend);

						Entity toEdit = systemManager->ecs->StartEditPrefab(newpath);
						PrefabWindow::prefabObj = toEdit.id;

					}

				}
				else if (check_extension(path.string(), ".gs")) {
					ImGui::Selectable(filename_string.c_str());

					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
						systemManager->mGameStateSystem->GetCurrentGameState()->Save();
						systemManager->mGameStateSystem->ChangeGameState(Misc::GetFileName(path.string()));
					}

				}

				else if (check_extension(path.string(), ".lua")) {

					ImGui::Selectable(filename_string.c_str());
					if (ImGui::BeginDragDropSource()) {

						std::string path_str = path.string();

						format_string(path_str);

						const char* source_path = path_str.c_str();
						ImGui::SetDragDropPayload("FILE_LUA", source_path, strlen(source_path) * sizeof(wchar_t), ImGuiCond_Once);

						ImGui::EndDragDropSource();
					}
				}


				else if (check_extension(path.string(), ".geom")) {
					ImGui::Selectable(filename_string.c_str());

					if (ImGui::BeginDragDropSource()) {

						std::string path_str = path.string();

						//format the string from \\ to /.
						format_string(path_str);
						const char* source_path = path_str.c_str();
						ImGui::SetDragDropPayload("FILE_GEOM", source_path, strlen(source_path) * sizeof(wchar_t), ImGuiCond_Once);

						ImGui::EndDragDropSource();
					}

				}
				else if (check_extension(path.string(), ".fbx")) {
					ImGui::Selectable(filename_string.c_str());

					if (ImGui::BeginDragDropSource()) {

						std::string path_str = path.string();

						//format the string from \\ to /.
						format_string(path_str);
						const char* source_path = path_str.c_str();
						ImGui::SetDragDropPayload("FILE_FBX", source_path, strlen(source_path) * sizeof(wchar_t), ImGuiCond_Once);

						ImGui::EndDragDropSource();
					}

				}
				else if (check_extension(path.string(), ".ctexture")) {
					ImGui::Selectable(filename_string.c_str());
					if (ImGui::BeginDragDropSource()) {

						std::string path_str = path.string();

						//format the string from \\ to /.
						format_string(path_str);
						const char* source_path = path_str.c_str();
						ImGui::SetDragDropPayload("FILE_TEXT", source_path, strlen(source_path) * sizeof(wchar_t), ImGuiCond_Once);

						ImGui::EndDragDropSource();
					}

				}
				else if (check_extension(path.string(), ".png") || check_extension(path.string(), ".jpg") || check_extension(path.string(), ".jpeg")) {
					ImGui::Selectable(filename_string.c_str());

					if (ImGui::BeginDragDropSource()) {

						std::string path_str = path.string();

						//format the string from \\ to /.
						format_string(path_str);
						const char* source_path = path_str.c_str();
						ImGui::SetDragDropPayload("FILE_TEXT_UNCOMPRESS", source_path, strlen(source_path) * sizeof(wchar_t), ImGuiCond_Once);

						ImGui::EndDragDropSource();
					}

				}
				else if (check_extension(path.string(), ".scn")) {
					ImGui::Selectable(filename_string.c_str());

					if (ImGui::BeginDragDropSource()) {

						std::string path_str = path.string();

						//format the string from \\ to /.
						format_string(path_str);
						const char* source_path = path_str.c_str();
						ImGui::SetDragDropPayload("FILE_SCN", source_path, strlen(source_path) * sizeof(wchar_t), ImGuiCond_Once);

						ImGui::EndDragDropSource();
					}

				}

				else if (check_extension(path.string(), ".wav")) {

					ImGui::Selectable(filename_string.c_str());

					if (ImGui::BeginDragDropSource()) {

						std::string path_str = path.string();

						int posstart = (int)path_str.find_last_of("\\");
						int posend = (int)path_str.find_last_of(".");

						std::string newpath = path_str.substr(posstart + 1, posend - (posstart + 1));

						//const char* audio_file_name = newpath.c_str();

						ImGui::SetDragDropPayload("FILE_AUDIO", path_str.c_str(), strlen(path_str.c_str()) * sizeof(wchar_t), ImGuiCond_Once);

						ImGui::EndDragDropSource();
					}
				}


				else if (check_extension(path.string(), ".mp3")) {

					ImGui::Selectable(filename_string.c_str());

					if (ImGui::BeginDragDropSource()) {

						std::string path_str = path.string();

						int posstart = (int)path_str.find_last_of("\\");
						int posend = (int)path_str.find_last_of(".");

						std::string newpath = path_str.substr(posstart + 1, posend - (posstart + 1));

						//const char* audio_file_name = newpath.c_str();

						ImGui::SetDragDropPayload("FILE_AUDIO", path_str.c_str(), strlen(path_str.c_str()) * sizeof(wchar_t), ImGuiCond_Once);

						ImGui::EndDragDropSource();
					}
				}
			}

			idd++;


		}
		ImGui::Columns(1);
		ImGui::EndTable();

	}
}
 
 
 
}














/***************************************************************************/



/*!
\brief
	Init Function for Imgui Window
*/
/**************************************************************************/
void ContentBrowser::init() {

	// empty
}

