/*!*************************************************************************
****
\file ContentBrowser.cpp
\author Han ChengWen
\par DP email: c.han@digipen.edu
\date 22-9-2022
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
#include <stdlib.h>
#include <iostream>
#include <string>
#include <algorithm>

static const std::filesystem::path s_asset_directory = "../assets";
bool ContentBrowser::dragCheck = false;

ContentBrowser::ContentBrowser():current_Directory(s_asset_directory) {} // constructor to set base directory


bool check_extension(std::string file, std::string extension) {
	size_t path_length = file.length();
	std::string path_extension = file.substr(path_length - extension.length());
	if (path_extension == extension)
		return true;

	return false;
}

void format_string(std::string& path) {

	std::replace(path.begin(), path.end(), '\\', '/');
}

void ContentBrowser::update() 
{
	if (current_Directory != std::filesystem::path(s_asset_directory))
	{
		if (ImGui::Button("<<",ImVec2(40,40)))
		{
			current_Directory = current_Directory.parent_path();
		}
	}

	static float padding{ 10 };
	static float buttonsize{ 200 };
	float cellsize = buttonsize + padding;

	float panelwidth = ImGui::GetContentRegionAvail().x;
	int columncount = (int)(panelwidth / cellsize);
	if (columncount < 1) {
		columncount = 1;
	}
	ImGui::Columns(columncount, 0, false);

	int idd{ 0 };
	// looping through filesystem
	for (auto const& directory : std::filesystem::directory_iterator{ current_Directory }) {
		const auto& path = directory.path(); // directory path

		auto relativepath = std::filesystem::relative(path, current_Directory);
		std::string filename_string = relativepath.filename().string();

		if (directory.is_directory()) {

			std::string file_button = filename_string.c_str();

			//ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			//ImGui::PushID(idd++);
			//ImGui::ImageButton((ImTextureID)(intptr_t)Texture2D::editor_Storage[file_button], { buttonsize, buttonsize });
			//ImGui::PopID();
			//ImGui::PopStyleColor();
			ImGui::Button(file_button.c_str(), { buttonsize, buttonsize }); // draw button of file

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				if (directory.is_directory())
					current_Directory /= path.filename();
			}
			//ImGui::Text(filename_string.c_str());
			
		}
		else{

			ImGui::Button(filename_string.c_str(), { buttonsize, buttonsize }); // draw button of file





			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				if (check_extension(path.string(), ".prefab")) {


					int posstart = filename_string.find_last_of("\\");
					int posend = filename_string.find_last_of(".");
					std::string newpath = filename_string.substr(posstart + 1,posend);

					Entity toEdit = systemManager->ecs->StartEditPrefab(newpath);
					PrefabWindow::prefabObj = toEdit.id;
				}


				//std::cout << path.string() << "\n";
			}

			if (check_extension(path.string(), ".lua")) {

				if (ImGui::BeginDragDropSource()) {

					std::string path_str = path.string();

					//format the string from \\ to /.
					format_string(path_str);
					//std::cout << path_str << "newstr\n";
					const char* source_path = path_str.c_str();
					ImGui::SetDragDropPayload("FILE_LUA", source_path, strlen(source_path) * sizeof(wchar_t), ImGuiCond_Once);

					ImGui::EndDragDropSource();
				}
			}


			if (check_extension(path.string(), ".geom")) {

				if (ImGui::BeginDragDropSource()) {

					std::string path_str = path.string();
					
					//format the string from \\ to /.
					format_string(path_str);
					const char* source_path = path_str.c_str();
					ImGui::SetDragDropPayload("FILE_GEOM", source_path, strlen(source_path) * sizeof(wchar_t), ImGuiCond_Once);

					ImGui::EndDragDropSource();
				}
			}
			if (check_extension(path.string(), ".ctexture")) {

				if (ImGui::BeginDragDropSource()) {

					std::string path_str = path.string();

					//format the string from \\ to /.
					format_string(path_str);
					const char* source_path = path_str.c_str();
					ImGui::SetDragDropPayload("FILE_TEXT", source_path, strlen(source_path) * sizeof(wchar_t), ImGuiCond_Once);

					ImGui::EndDragDropSource();
				}
			}

			if (check_extension(path.string(), ".prefab")) {
	

				if (ImGui::BeginDragDropSource()) {

					std::string path_str = path.string();

					//format the string from \\ to /.
					//format_string(path_str);
					int posstart = path_str.find_last_of("\\");
					int posend = path_str.find_last_of(".");

					std::string newpath = path_str.substr(posstart + 1, posend - (posstart + 1));
					
					const char* source_path = newpath.c_str();
					ImGui::SetDragDropPayload("FILE_PREFAB", source_path, strlen(source_path) * sizeof(wchar_t), ImGuiCond_Once);

					ImGui::EndDragDropSource();
				}
			}

		}

		ImGui::NextColumn();
	}
	ImGui::Columns(1);
}


void ContentBrowser::init() {

	// empty
}

