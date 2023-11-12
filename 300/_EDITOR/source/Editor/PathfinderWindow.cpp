#include "PathfinderWindow.h"
#include "imgui_stdlib.h"
#include "AI/PathfinderManager.h"
#include "Hierarchy.h"
#include "EditorReflection/EditorReflection.h"
bool PathfinderWindow::openWindow;


void PathfinderWindow::init() {
	openWindow = false;
  mLatestPoint.id = entt::null;
  mLatestEdge.id = entt::null;
}


void PathfinderWindow::update() {
  PathfinderManager* pfSys = systemManager->GetPathfinderManager();

  // Update entity history
  if (Entity(Hierarchy::selectedId).HasComponent<General>()) {
    General& genComp = Entity(Hierarchy::selectedId).GetComponent<General>();
    if (genComp.GetTag() == "GRAPH") {

      if (genComp.name == POINTNAME) {
        mLatestPoint = Entity(Hierarchy::selectedId);

        if (mEdgePoints.size()) {
          if (mEdgePoints.back() != Entity(Hierarchy::selectedId)) {
            mEdgePoints.push(Entity(Hierarchy::selectedId));
            if (mEdgePoints.size() > 2) mEdgePoints.pop();
          }
        } else mEdgePoints.push(Entity(Hierarchy::selectedId));
      }
      else if (genComp.name == EDGENAME) mLatestEdge = Entity(Hierarchy::selectedId);
    }
  }

  // UI portion
  //------------------------------
  // Add new graph data
  static std::string tempGraphDataName;
  ImGui::InputText("New Graph Data", &tempGraphDataName);
  if (ImGui::Button("Create New Graph Data")) {
    pfSys->NewGraphData(tempGraphDataName);
    tempGraphDataName = "";
  }

  if (pfSys->mActiveGraph >= 0) {
    // Delete currently active graph data
    if (ImGui::Button("Delete Graph Data")) {
      pfSys->DeleteActiveGraphData();
    }
    // Save currently active graph data
    if (ImGui::Button("Save Graph Data")) {
      pfSys->SaveActiveGraphData();
    }
    // Refresh the data of the waypoint entities
    if (ImGui::Button("Refresh Entities")) {
      pfSys->RefreshGraphEntity();
      mLatestPoint.id = entt::null;
      mLatestEdge.id = entt::null;
      while (mEdgePoints.size()) mEdgePoints.pop();
    }
  }


  // Selecting active graph (drop down list)
  std::string activeGraphDataName = pfSys->mActiveGraph < 0 ? "" : pfSys->mGraphDataNameList[pfSys->mActiveGraph];
  if (ImGui::BeginCombo("Active Graph Data", activeGraphDataName.c_str())) {
    for (int i{}; i < pfSys->mGraphDataNameList.size(); ++i) {
      if (ImGui::Selectable(pfSys->mGraphDataNameList[i].c_str())) {
        pfSys->SetActiveGraphData(pfSys->mGraphDataNameList[i]);
        pfSys->mActiveGraph = i;
      }
    }
    ImGui::EndCombo();
  }

  if (pfSys->mActiveGraph == -1) return;
  SetCursor();

  // Draw graph toggle
  static bool drawHistory{ true };
  ImGui::Checkbox("Draw Graph", &(pfSys->mDrawGraph));
  if (drawHistory != pfSys->mDrawGraph) {
    pfSys->mDrawGraph = drawHistory;
    pfSys->ToggleDrawGraph();
    drawHistory = pfSys->mDrawGraph;
  }

  // Point tool
  ImGui::Text("Point Editing Tool");
  // Currently selected point, vec3 adjustable
  ImGui::Text("Selected Point: ");
  ImGui::SameLine();
  if (mLatestPoint.id == entt::null) ImGui::Text("No active point selected");
  else ImGui::DragFloat3("##CurrentSelectedPoint", (float*)&mLatestPoint.GetComponent<Transform>().mTranslate, 0.1f);
    
  ImGui::Separator();

  // Add point (button and coordinate input)
  static glm::vec3 tempTransform{};
  ImGui::Text("New Point");
  ImGui::SameLine();
  ImGui::DragFloat3("##NewPoint", (float*)&tempTransform, 0.1f);
  if (ImGui::Button("Create Point")) {
    pfSys->AddPoint(tempTransform);
    tempTransform = glm::vec3();
  }

  // Delete Point (button) (deletes latest selected point)
  if (ImGui::Button("Delete Point")) {
    pfSys->DeleteEntity(mLatestPoint);
    mLatestPoint.id = entt::null;
  }

  // Edge Tool

  // Adding edge portion
  ImGui::Text("Add Edge Tool");

  // Display selected points for edge making
  ImGui::Text("Start Point: ");
  ImGui::SameLine();
  if (mEdgePoints.size() >= 1) {
    glm::vec3 point = mEdgePoints.front().GetComponent<Transform>().mTranslate;
    ImGui::DragFloat3("##AddEdgeStartPoint", (float*)&point, 0.1f);
  } else {
    ImGui::Text("No Start Point Selected");
  }

  ImGui::Text("End Point: ");
  ImGui::SameLine();
  if (mEdgePoints.size() >= 2) {
    glm::vec3 point = mEdgePoints.back().GetComponent<Transform>().mTranslate;
    ImGui::DragFloat3("##AddEdgeEndPoint", (float*)&point, 0.1f);
  } else {
    ImGui::Text("No End Point Selected");
  }

  // Add Direccted edge (button) (should reset the display point)
  // Add Undirected edge (button) (should reset the display point)
  if (mEdgePoints.size() == 2) {
    if (ImGui::Button("Add Directed Edge")) {
      pfSys->AddDirectedEdge(mEdgePoints.front(), mEdgePoints.back());
      mEdgePoints.pop();
      mEdgePoints.pop();
    }
    if (ImGui::Button("Add Undirected Edge")) {
      pfSys->AddUndirectedEdge(mEdgePoints.front(), mEdgePoints.back());
      mEdgePoints.pop();
      mEdgePoints.pop();
    }
  }
  ImGui::Separator();

  // Delete edge tool
  // Delete edge (button) (deletes every edge between the display)
  // Display start point
  // Display end point

  ImGui::Text("Delete Edge Tool");
  if (mLatestEdge.id == entt::null) ImGui::Text("No active edge selected");
  else {
    std::vector<glm::vec3> startnend = pfSys->GetEdgeEnds(mLatestEdge);

    ImGui::Text("Start Point");
    ImGui::SameLine();
    ImGui::DragFloat3("##StartPoint", (float*)&startnend.front(), 0.1f);

    ImGui::Text("End Point");
    ImGui::SameLine();
    ImGui::DragFloat3("##EndPoint", (float*)&startnend.back(), 0.1f);

    if (ImGui::Button("Delete Directed Edges")) {
      pfSys->DeleteEntity(mLatestEdge);
      mLatestEdge.id = entt::null;
    }
  }


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

