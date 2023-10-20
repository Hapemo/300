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
  if (ImGui::TreeNodeEx("Keybind Mapping", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick)) {
    for (auto& [action, ekey] : *keybindMap) {
      ImGui::Text(action.c_str());
      ImGui::SameLine();

      if (ImGui::InputText("Name", &inputSystem->GetActionEKeyName(action)));

      //if (ImGui::BeginCombo("##combo", ekeyName.c_str())) // The second parameter is the label previewed before opening the combo.
      //{
      //  while (ekeyRecurr != ekeyMap->end()) {
      //    bool is_selected = (ekeyName == ekeyRecurr->first); // You can store your selection however you want, outside or inside your objects
      //    if (ImGui::Selectable(ekeyRecurr->first.c_str(), is_selected))
      //      ekeyName = ekeyRecurr->first;
      //    if (is_selected)
      //      ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
      //    ++ekeyRecurr;
      //  }
      //  //for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
      //  //  bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
      //  //  if (ImGui::Selectable(items[n], is_selected)
      //  //      current_item = items[n];
      //  //      if (is_selected)
      //  //        ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
      //  //}
      //  ImGui::EndCombo();
      //}

    }

    ImGui::TreePop();
  }
}

