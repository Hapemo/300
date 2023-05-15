/*!*****************************************************************************
\file DialogueScript.cpp
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 23-11-2022
\brief
The DialogueScript handles the dialogue functionalities such as proceeding to
the next line of dialogue and the zooming in of the camera during the dialogue
scene.
*******************************************************************************/

#include "DialogueScript.h"

REGISTER_SCRIPT(ScriptComponent, DialogueScript);

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity is activated.
*******************************************************************************/
void DialogueScript::Alive(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on initialisation of the entity.
*******************************************************************************/
void DialogueScript::Init(Entity const& _e) {
	(void)_e;
	//LOG_INFO("Dialogue script starts works!!!");
	VI::iDialogue::LoadScript("Dialogue LittleGirl 0");
	VI::iDialogue::SetCurrentId(1);
	if (_e.HasComponent<Text>()) _e.GetComponent<Text>().text = VI::iDialogue::GetLine(VI::iDialogue::GetCurrentId());
	(VI::iEntity::GetEntity("DialogueBox", "Level1")).Activate();
	currScn = &(VI::iScene::Select("Level1"));
	currCamera = &VI::iCamera::CurrentCamera();
	initialCamScale = VI::iCamera::CurrentCamera().scale;
	currCamScale = initialCamScale; 
	targetCamScale = initialCamScale * 0.5f;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active.
*******************************************************************************/
void DialogueScript::EarlyUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate functions from all other active scripts.
*******************************************************************************/
void DialogueScript::Update(Entity const& _e) {
	currCamera->scale = currCamScale;
	if (currCamera->scale.x >= targetCamScale.x)
		currCamera->scale.x -= 500 * (float)FUNC->GetDeltaTime();
	currCamScale = currCamera->scale;
	if (VI::iInput::CheckKey(E_STATE::PRESS, M_BUTTON_L)) {
		if (_e.HasComponent<Text>()) {
			int currentId = VI::iDialogue::GetCurrentId();
			if (!VI::iDialogue::SetCurrentId(++currentId)) {
				_e.GetComponent<Text>().text = "";
				_e.Deactivate();
			} else {
				_e.GetComponent<Text>().text = VI::iDialogue::GetLine(currentId);
				//FUNC->PlaySoundInChannel("BTNCLICK", (int)E_AUDIO_CHANNEL::FORCEPLAY);
			}
		}
	}

	if (VI::iInput::CheckKey(HOLD, LEFT_CONTROL) && VI::iInput::CheckKey(HOLD, LEFT_SHIFT) && VI::iInput::CheckKey(PRESS, N)) {
		_e.GetComponent<Text>().text = "";
		_e.Deactivate();
	}
}

/*!*****************************************************************************
\brief
Function will run on fixed delta time after running all EarlyUpdate and Update
functions from all other active scripts.
*******************************************************************************/
void DialogueScript::FixedUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate, Update, and FixedUpdate functions from all other active scripts.
*******************************************************************************/
void DialogueScript::LateUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on exit or when the entity is destroyed.
*******************************************************************************/
void DialogueScript::Exit(Entity const& _e) {
	(void)_e;
	//LOG_INFO("Dialogue script end works!!!");
	(VI::iEntity::GetEntity("DialogueBox", "Level1")).Deactivate();
}

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity exits.
*******************************************************************************/
void DialogueScript::Dead(Entity const& _e) {
	(void)_e;
}
