#include "KeybindWindow.h"
#include "Input/InputMapSystem.h"
#include "imgui_stdlib.h"

bool KeybindWindow::openWindow;

void KeybindWindow::init() {
	openWindow = false;
}


void KeybindWindow::update() {
  auto inputSystem = systemManager->GetInputMapSystemPointer();

  auto keybindMap = inputSystem->GetKeybindMap();

  // Need to print 2 columns, action and the keyName inputable by imgui.
  // Another button to save the keybind. This will save the keybind to the file and also
  // update all existing keys.
  // 

  std::string toDelete{};

  if (ImGui::TreeNodeEx("Keybind Mapping", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick)) {
    for (auto& [action, ekey] : *keybindMap) {
      //ImGui::Text(action.c_str());

      ImGui::InputText(action.c_str(), &inputSystem->GetActionEKeyName(action));

      if (ImGui::Button("Delete"))
        toDelete = action;
    }

    ImGui::TreePop();
  }

  if (toDelete.size()) inputSystem->RemoveKeybind(toDelete);

  if (ImGui::Button("Save"))
    inputSystem->SaveKeybind();
  static std::string tempActionName;
  ImGui::InputText("New action name", &tempActionName);
  if (ImGui::Button("Add Action")) {
    inputSystem->AddEmptyKeybind(tempActionName);
    tempActionName = "";
  }
}

