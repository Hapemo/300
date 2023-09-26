#include "PrefabWindow.h"
#include "ECS/ECS_Components.h"

entt::entity PrefabWindow::prefabObj;


void PrefabWindow::init() {
	prefabObj = static_cast<entt::entity>(- 1);
}

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
		if (ent.HasComponent<PlaneCollider>()) {
			PlaneCollider& planeCollider = ent.GetComponent<PlaneCollider>();
			planeCollider.Inspect();
		}
	}


	if (ImGui::Button("Save")) {

		Entity ent(PrefabWindow::prefabObj);
		systemManager->ecs->EndEditPrefab(ent);

		PrefabWindow::prefabObj = static_cast<entt::entity>(-1);
	}

	ImGui::SameLine();
	if (ImGui::Button("Delete")) {
		Entity ent(PrefabWindow::prefabObj);
		systemManager->ecs->DeleteEntity(ent);
		PrefabWindow::prefabObj = static_cast<entt::entity>(-1);
	}

	
}