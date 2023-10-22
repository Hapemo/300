/**-------------------------------------------------------------------------------------/
 * @file
 *  PrefabWindow.cpp
 * @author
 *  Han Cheng Wen, c,han@digipen.edu
 * @date
 *  2023/05/21
 * @brief
 *  Prefab Scene function definition
 * @copyright
 *  Copyright (C) 2023 DigiPen Institute of Technology.
 *-------------------------------------------------------------------------------------*/
#include "PrefabWindow.h"
#include "ECS/ECS_Components.h"

entt::entity PrefabWindow::prefabObj;

/***************************************************************************/
/*!
\brief
	Init function for Prefab window
*/
/***************************************************************************/
void PrefabWindow::init() {
	prefabObj = static_cast<entt::entity>(0);
}
/***************************************************************************/
/*!
\brief
	Update function for Menupanel window
*/
/***************************************************************************/
void PrefabWindow::update() {


	if (static_cast<int>(prefabObj) != -1) {

		Entity ent(PrefabWindow::prefabObj);

		if (ent.HasComponent<Transform>()) {
			Transform& transform = ent.GetComponent<Transform>();
			transform.Inspect();
		}
		if (ent.HasComponent<Scripts>())
		{
			Scripts& scripts = ent.GetComponent<Scripts>();
			scripts.Inspect();
		}

		if (ent.HasComponent<MeshRenderer>()) {
			MeshRenderer& Meshrender = ent.GetComponent<MeshRenderer>();
			Meshrender.Inspect();
		}
		if (ent.HasComponent<RigidBody>()) {
			RigidBody& rigidBody = ent.GetComponent<RigidBody>();
			rigidBody.Inspect();
		}
		if (ent.HasComponent<BoxCollider>()) {
			BoxCollider& boxCollider = ent.GetComponent<BoxCollider>();
			boxCollider.Inspect();
		}
		if (ent.HasComponent<SphereCollider>()) {
			SphereCollider& sphereCollider = ent.GetComponent<SphereCollider>();
			sphereCollider.Inspect();
		}



		if (ImGui::Button("Save")) {

			Entity ent1(PrefabWindow::prefabObj);
			systemManager->ecs->EndEditPrefab(ent1);

			PrefabWindow::prefabObj = static_cast<entt::entity>(0);
		}

		ImGui::SameLine();
		if (ImGui::Button("Delete")) {
			Entity ent2(PrefabWindow::prefabObj);
			systemManager->ecs->DeleteEntity(ent2);
			PrefabWindow::prefabObj = static_cast<entt::entity>(0);
		}
	}




	
}