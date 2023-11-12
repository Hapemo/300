#include "PathfinderWindow.h"
#include "Input/InputMapSystem.h"
#include "imgui_stdlib.h"

bool PathfinderWindow::openWindow;

void PathfinderWindow::init() {
	openWindow = false;
}


void PathfinderWindow::update() {
  //auto inputSystem = systemManager->GetInputMapSystemPointer();

  //auto keybindMap = inputSystem->GetKeybindMap();

  //// Need to print 2 columns, action and the keyName inputable by imgui.
  //// Another button to save the keybind. This will save the keybind to the file and also
  //// update all existing keys.
  //// 

  //std::string toDelete{};
  //int id{ 0 };
  //if (ImGui::TreeNodeEx("Keybind Mapping", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick)) {
  //  for (auto& [action, ekey] : *keybindMap) {
  //    ImGui::InputText(action.c_str(), &inputSystem->GetActionEKeyName(action));

  //    ImGui::SameLine();
  //    
  //    ImGui::PushID(id);
  //    if (ImGui::Button("Delete")) toDelete = action;
  //    ImGui::PopID();
  //    id++;
  //  }

  //  ImGui::TreePop();
  //}

  //if (toDelete.size()) inputSystem->RemoveKeybind(toDelete);

  //if (ImGui::Button("Save"))
  //  inputSystem->SaveKeybind();
  //static std::string tempActionName;
  //ImGui::InputText("New action name", &tempActionName);
  //if (ImGui::Button("Add Action")) {
  //  inputSystem->AddEmptyKeybind(tempActionName);
  //  tempActionName = "";
  //}
}

