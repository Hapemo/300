#include "TabWindow.h"
#include "Graphics/GraphicsSystem.h"
#include "ECS/ECS.h"
#include "ECS/ECS_Systems.h"

void TabWindow::init() {
    //mWinFlag |= ImGuiWindowFlags_NoBringToFrontOnFocus
    //    | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar
    //    | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar|ImGuiDockNodeFlags_AutoHideTabBar | 
    //    ImGuiDockNodeFlags_PassthruCentralNode;
}

void TabWindow::update() {

	glm::vec3 mTranslate{};
	glm::vec3 mRotate{};
	float speed{};
	ImGui::Text("Position");
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcItemWidth()
		- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
	ImGui::DragFloat3("##Position", (float*)&mTranslate);

	ImGui::Separator();
	ImGui::Text("Rotation");
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcItemWidth()
		- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
	ImGui::DragFloat3("##Rotation", (float*)&mRotate);

	ImGui::Separator();
	ImGui::Text("Camera Speed");
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcItemWidth()
		- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
	ImGui::DragFloat("##CS", (float*)&speed);

	ImGui::Separator();
	ImGui::Text("Debug");
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcItemWidth()
		- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
	static bool ischecked = systemManager->mGraphicsSystem->m_DebugDrawing;
	if (ImGui::Checkbox("##Debug", &ischecked)) {
		systemManager->mGraphicsSystem->m_DebugDrawing = ischecked ? 1 : 0;
	}

}