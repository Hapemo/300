/*!*****************************************************************************
/*!*****************************************************************************
\file WorldViewPanel.cpp
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Group: Memory Leak Studios
\date 20-10-2022
\brief
This file contains function definations for a WorldView Panel Editor system that displays the world scene
*******************************************************************************/
#include "WorldViewPanel.h"
#include <ECSManager.h>
#include "GameStateManager.h"
/*!*****************************************************************************
\brief
	Initializes the WorldViewPanel editor

\return
None.
*******************************************************************************/
void WorldViewPanel::Init()
{
	viewportSize={ 0,0 };
	//camera = renderManager->GetWorldCamera();
}
/*!*****************************************************************************
\brief
	Updates the WorldViewPanel Panel editor

\return
None.
*******************************************************************************/
void WorldViewPanel::Update()
{
	if (ImGui::Begin("World View"))
	{
		//Calcualting the view port aspect ratio 
		SetViewportAspectRatio();
		CalculateMousePos(E_CAMERA_TYPE::WORLD);
		mWorldMousePos = worldMousePos;
		isViewportPaused = isScenePaused;
		//ImGui::SetWindowFontScale(1.1);
		if (selectedGameState < (*mGameStates).size())
		{
			std::string gamesstateName = "GameState: " + (*mGameStates)[selectedGameState].mName;
			ImGui::Text(gamesstateName.c_str());
			if (selectedScene < (*mGameStates)[selectedGameState].mScenes.size())
			{
				std::string sceneName= "Scene : " + (*mGameStates)[selectedGameState].mScenes[selectedScene].mName;
				ImGui::Text(sceneName.c_str());
			}
		}
		else if (selectedScene == 99)
		{
			std::string sceneName = "NO SCENE SELECTED!";
			ImGui::Text(sceneName.c_str());
		}
		//ImGui::SetWindowFontScale(1.0);
		renderUI();
		ImGui::SetWindowFontScale(1.3f);
		ImGui::SameLine(0.f, 30.f);
		if (ImGui::Button("Reset", buttonSize))
		{
			//std::string oldName = (*mGameStates)[selectedGameState].mName;
			std::string name = "../resources/GameStates/";
			name += (*mGameStates)[selectedGameState].mName;
			name += ".json";
			std::filesystem::path path{ name };
			//(*mGameStates)[selectedGameState].mName = "Pls Delete me";
			SceneReset();
			GameStateManager::GetInstance()->RemoveGameState(&(*mGameStates)[selectedGameState]);
			GameStateManager::GetInstance()->AddGameState(path);
			SceneReset();
			//GameStateManager::GetInstance()->SetGameState(path.stem().string());
			selectedGameState = (int)(*mGameStates).size() - 1;
			selectedScene = (int)(*mGameStates)[selectedGameState].mScenes.size() - 1;

			/*selectedScene = 100;
			selectedGameState = 100;*/
		}
		ImGui::SetWindowFontScale(1.0f);
		static bool checkSave = false;
		if (!isViewportPaused && isScenePaused && undoStack.size() != 0)
		{
			isScenePaused = isViewportPaused = true;
			checkSave = true;
		}
		else
		{
			if (!isViewportPaused)
				SceneReset();
			isScenePaused = isViewportPaused;
		}

		//isScenePaused = isViewportPaused;
		CalculateMousePos(E_CAMERA_TYPE::WORLD);
		fameBufferImage = (void*)(intptr_t)renderManager->GetWorldFBO();
		ImGui::SetCursorPos(ImVec2(viewportPos.x, viewportPos.y));
		ImGui::Image(fameBufferImage, { viewportSize.x, viewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));
		NewDragDropEntity();
		/*if (ImGui::BeginDragDropTarget())
		{
			NewEntity();
			ImGui::EndDragDropTarget();
		}*/
		if (ImGui::BeginDragDropTarget())
		{
			NewPrefabee();
			ImGui::EndDragDropTarget();
		}
		if (checkSave)
		{
			ImGui::SetWindowFontScale(1.5f);
			ImGui::SetCursorPos(ImVec2(50.f, ImGui::GetWindowHeight()/4.f));
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 200)));
			ImGui::Button("Do you want to save scene first?", ImVec2(ImGui::GetWindowWidth() - 100.f, ImGui::GetWindowHeight() / 5.f));
			ImGui::PopStyleColor();
			ImGui::SetWindowFontScale(1.2f);
			ImGui::SetCursorPos(ImVec2(50.f, ImGui::GetWindowHeight() / 2.f));
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 200, 0)));
			if (ImGui::Button("YES!! Thank you for reminding me.", ImVec2((ImGui::GetWindowWidth() - 110.f)/2.f, ImGui::GetWindowHeight() / 10.f)))
			{
				(*mGameStates)[selectedGameState].Save();
				isScenePaused = isViewportPaused = false;
				SceneReset();
				checkSave = false;
			}
			ImGui::PopStyleColor();
			ImGui::SameLine(0.f, 10.f);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(200, 0, 0)));
			if (ImGui::Button("No.. Just testing.", ImVec2((ImGui::GetWindowWidth() - 110.f)/2.f, ImGui::GetWindowHeight() / 10.f)))
			{
				isScenePaused = isViewportPaused = false;
				SceneReset();
				checkSave = false;
			}
			ImGui::PopStyleColor();
			ImGui::SetWindowFontScale(1.0f);
			
		}
		PopObject();
		if (ImGui::IsWindowHovered())
		{
			if (IsMouseInScreen())
			{
				if (selectedScene == 99 && isScenePaused)
				{
					if (Input::CheckKey(E_STATE::PRESS, E_KEY::M_BUTTON_L) || Input::CheckKey(E_STATE::HOLD, E_KEY::M_BUTTON_L))
					{
						ImGui::SetWindowFontScale(1.8f);
						ImGui::SetCursorPos(ImVec2(50.f, ImGui::GetWindowHeight() / 2.f-200.f));
						ImGui::Button("<----\t\t\tSelect a Scene! \n This is a preview of all Active scenes overlayed.", ImVec2(ImGui::GetWindowWidth()-100.f,400.f));
						ImGui::SetWindowFontScale(1.0f);
					}
				}
				//Camera movement
				ArrowKeyMoveCam();
				ScrollMoveCam();
				if (isScenePaused)
				{
					MouseClickMoveCam();
					if(!SRT)
						MouseOverObject();
				}
				else
				{
					/*if (Input::CheckKey(E_STATE::PRESS, E_KEY::M_BUTTON_L) || Input::CheckKey(E_STATE::HOLD, E_KEY::M_BUTTON_L))
					{
						ImGui::SetWindowFontScale(1.5f);
						ImGui::SetCursorPos(ImVec2(50.f, ImGui::GetWindowHeight() / 2.f - 200.f));
						ImGui::Button("Dont touch me when scene is playing!!", ImVec2(ImGui::GetWindowWidth() - 100.f, 300.f));
						ImGui::SetWindowFontScale(1.0f);
					}*/
				}

			}
			if (Input::CheckKey(E_STATE::RELEASE, E_KEY::M_BUTTON_L) && isSelected == 1)
			{   ///save changes for undo
				COMPONENT translate = e.GetComponent<Transform>();
				undoStack.push_back(std::make_pair(e, translate));
				stackPointer = (int)undoStack.size();
			}
			if (Input::CheckKey(E_STATE::NOTPRESS, E_KEY::M_BUTTON_L))
				isSelected = 0;
			
		
			//object picking
			if (Input::CheckKey(E_STATE::PRESS, E_KEY::M_BUTTON_L) && IsMouseInScreen() &&SRT == 0)
			{
				if (isScenePaused)
					SetSelectedEntity();
			}

			if (selectedEntity != nullptr && (*selectedEntity).GetComponent<General>().tag != TAG::BACKGROUND)
			{
				if (isScenePaused)
				{
					e = *selectedEntity;
					if (SRT == 0)
					{
						MoveSelectedEntity();
					}
					else if (SRT == 4)
						renderManager->GetGizmo().Attach(*selectedEntity);
					else if (SRT == 5)
						pointPicker();
					else if (SRT == 6)
						pointPicker();
					else
						UseGuizmo();
				}
			}
			if(SRT!=4)
				renderManager->GetGizmo().Detach();
		}
	}
	ImGui::End();
	
}
/*!*****************************************************************************
\brief
	Free the WorldViewPanel editor

\return
None.
*******************************************************************************/
void WorldViewPanel::Free()
{

}
/*!*****************************************************************************
\brief
	This Function Moves the camera based on arrow key input

\return
None.
*******************************************************************************/
void WorldViewPanel::ArrowKeyMoveCam()
{
	if (Input::CheckKey(E_STATE::HOLD, E_KEY::UP))
	{
		renderManager->GetWorldCamera() += moveVertical;
	}
	if (Input::CheckKey(E_STATE::HOLD, E_KEY::DOWN))
	{
		renderManager->GetWorldCamera() -= moveVertical;
	}
	if (Input::CheckKey(E_STATE::HOLD, E_KEY::RIGHT))
	{
		renderManager->GetWorldCamera() += moveHorizontal;
	}
	if (Input::CheckKey(E_STATE::HOLD, E_KEY::LEFT))
	{
		renderManager->GetWorldCamera() -= moveHorizontal;
	}
}
/*!*****************************************************************************
\brief
	This Function Moves the camera based on mouse input

\return
None.
*******************************************************************************/
void WorldViewPanel::MouseClickMoveCam()
{
	if (SRT == 0)
	{
		if (Input::CheckKey(E_STATE::PRESS, E_KEY::M_BUTTON_R) && !isSelected)
		{
			camOffset = camMousePos;
		}
		if (Input::CheckKey(E_STATE::HOLD, E_KEY::M_BUTTON_R) && !isSelected)
		{
			camPos = -(worldMousePos * renderManager->GetWorldCamera().GetZoom() - camOffset);
			renderManager->GetWorldCamera().SetPos(camPos);
		}
	}
}
/*!*****************************************************************************
\brief
	This Function Zoom the camera based on mouse input 

\return
None.
*******************************************************************************/
void WorldViewPanel::ScrollMoveCam()
{
	if (Input::GetScroll() > 0.0) //scroll up   // zoon in
	{

		renderManager->GetWorldCamera() += worldMousePos * moveZoom;
		renderManager->GetWorldCamera() *= -moveZoom;
		//renderManager->GetWorldCamera() -= -mousePos;
	}
	else if (Input::GetScroll() < 0.0)  //scroll down //zoom out
	{
		renderManager->GetWorldCamera() -= worldMousePos * moveZoom;
		renderManager->GetWorldCamera() *= moveZoom;
		//renderManager->GetWorldCamera() += -mousePos;
	}
}
/*!*****************************************************************************
\brief
	This Function created a new enitity

\return
None.
*******************************************************************************/
void WorldViewPanel::NewDragDropEntity()
{
	if (ImGui::BeginDragDropTarget())
	{
		static const wchar_t* texpath = (const wchar_t*)"";
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURES"))
		{
			if (selectedGameState >= (*mGameStates).size())
			{
				NewGameState();
			}
			if (selectedScene >= (*mGameStates)[selectedGameState].mScenes.size())
			{
				NewScene();
			}

			LOG_INFO("New DragDrop Entity");
			e = NewEntity();
			e.GetComponent<General>().isPaused = false;
			e.GetComponent<Transform>().translation = camMousePos;
			e.GetComponent<Sprite>().sprite = SPRITE::TEXTURE;

			texpath = (const wchar_t*)payload->Data;
			std::string tp = (std::string)((const char*)texpath);
			spriteManager->SetTexture(e, tp);

			/*Entity ne{ ECS::CreateEntity() };
			(*mGameStates)[selectedGameState].mScenes[selectedScene].mEntities.insert(ne);
			ne.AddComponent(
				General{ "_NEW_DragDrop" + std::to_string(newEntityCount), TAG::OTHERS, SUBTAG::NOSUBTAG, true , false},
				Transform{ {150,150}, 0, camMousePos },
				Sprite{ Color{0,255,0,100}, SPRITE::TEXTURE, 0, highestLayer},
				RectCollider{ { 0.f, 0.f }, {1.f,1.f}, true });

			texpath = (const wchar_t*)payload->Data;
			std::string tp = (std::string)((const char*)texpath);
			spriteManager->SetTexture(ne, tp);
			newEntityCount++;*/
		}
		ImGui::EndDragDropTarget();
	}
}
/*!*****************************************************************************
\brief
	This Function created a new prefabee

\return
None.
*******************************************************************************/
void WorldViewPanel::NewPrefabee()
{
	static const wchar_t* prefabname = (const wchar_t*)"";
	if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PREFAB"))
	{
		//FUNCTION GS SCENE
		if (selectedGameState >= (*mGameStates).size())
		{
			NewGameState();
		}
		if (selectedScene >= (*mGameStates)[selectedGameState].mScenes.size())
		{
			NewScene();
		}

		LOG_INFO("Created new prefabee");
		prefabname = (const wchar_t*)payload->Data;
		std::string prefab = (std::string)((const char*)prefabname);
		serializationManager->LoadPrefab(prefab);
		PrefabManager::PrefabPtr pre = PrefabManager::GetInstance()->GetPrefab(prefab);
		static int newPrefabeeCount{ 1 };
		Entity b = pre->CreatePrefabee();
		b.GetComponent<General>().name = "NEW Prefabee" + std::to_string(newPrefabeeCount++);
		b.GetComponent<General>().isActive = true;  //?? needed?
		b.GetComponent<General>().isPaused = false;
		if (b.HasComponent<Transform>())
		{
			b.GetComponent<Transform>().translation = camMousePos;
			/*b.GetComponent<Transform>().translation.x += prefabOffset;
			b.GetComponent<Transform>().translation.y += prefabOffset;
			prefabOffset += 10.f;*/
		}
		/*if (b.HasComponent<Sprite>())
		{
			b.GetComponent<Sprite>().layer = highestLayer;
		}*/
		((*mGameStates)[selectedGameState].mScenes[selectedScene].mEntities).insert(b);
	}
	
}
/*!*****************************************************************************
\brief
	This Function sets the selected entity

\return
None.
*******************************************************************************/
void WorldViewPanel::SetSelectedEntity()
{
	int layer = 0;
	if (!(selectedGameState < (*mGameStates).size() && selectedScene < (*mGameStates)[selectedGameState].mScenes.size()))
		return;
	for (const Entity& ee : (*mGameStates)[selectedGameState].mScenes[selectedScene].mEntities)
	{
		if (ee.GetComponent<General>().tag == TAG::BACKGROUND)
			continue;
		if (ee.HasComponent<Transform>() && ee.HasComponent<Sprite>()) //||e.HasComponent<Text>()
		{
			Math::Vec2 scale = ee.GetComponent<Transform>().scale;
			Math::Vec2 translation = ee.GetComponent<Transform>().translation;
			Math::Vec2 distance = camMousePos - translation;
			if (abs(distance.x) < abs(scale.x) / 2 && abs(distance.y) < abs(scale.y) / 2)
			{
				LOG_INFO(ee.GetComponent<General>().name + " Selected");
				if (ee.GetComponent<Sprite>().layer >= layer)
				{
					selectedEntity = &ee;
					renderManager->SelectEntity(*selectedEntity);
					COMPONENT translate = ee.GetComponent<Transform>();
					undoStack.push_back(std::make_pair(ee, translate));
					stackPointer = (int)undoStack.size();
					layer = ee.GetComponent<Sprite>().layer;
					objectOffset = distance;
					isSelected = 1;
					aspect = false;
				}
			}
		}
	}
}
/*!*****************************************************************************
\brief
	This Function checks the mover over entity

\return
None.
*******************************************************************************/
void WorldViewPanel::MouseOverObject()
{
	int layer = 0;
	bool notColliding = false;
	if (!(selectedGameState < (*mGameStates).size() && selectedScene < (*mGameStates)[selectedGameState].mScenes.size()))
		return;
	const Entity* eee = nullptr;
	for (const Entity& ee : (*mGameStates)[selectedGameState].mScenes[selectedScene].mEntities)
	{
		if (ee.GetComponent<General>().tag == TAG::BACKGROUND)
			continue;
		if (ee.HasComponent<Transform>() && ee.HasComponent<Sprite>()) //||e.HasComponent<Text>()
		{
			Math::Vec2 scale = ee.GetComponent<Transform>().scale;
			Math::Vec2 translation = ee.GetComponent<Transform>().translation;
			Math::Vec2 distance = camMousePos - translation;
			if (abs(distance.x) < abs(scale.x) / 2 && abs(distance.y) < abs(scale.y) / 2)
			{
				if (ee.GetComponent<Sprite>().layer >= layer)
				{
					notColliding = true;
					layer = ee.GetComponent<Sprite>().layer;
					bool check = true;
					for (int i = (int)popList.size() - 1; i >= 0; i--)
					{
						if ((popList[i].first->id == ee.id))
						{
							check = false;
							break;
						}
					}
					if (check)
					{
						eee = &ee;
						/*if (pop != nullptr)
						{
							if(pop->id != ee.id)
								popList.push_back(std::make_pair(&ee, 0));
						}
						else
							popList.push_back(std::make_pair(&ee, 0));*/

					}
				}
			}
		}
	}
	if (eee != nullptr)
	{
		if (pop != nullptr)
		{
			if (pop->id != eee->id)
				popList.push_back(std::make_pair(eee, 0));
		}
		else
			popList.push_back(std::make_pair(eee, 0));

	}
	if (!notColliding)
		pop = nullptr;
}
void WorldViewPanel::PopObject()
{
	for (int i = 0; i < popList.size(); i++)
	{
		if (popList[i].first != nullptr)
		{
			if (popList[i].second < 7)
			{
				popList[i].first->GetComponent<Transform>().scale.x += 2.5f;
				popList[i].first->GetComponent<Transform>().scale.y += 2.5f;
			}
			else if(popList[i].second < 14)
			{
				popList[i].first->GetComponent<Transform>().scale.x -= 2.5f;
				popList[i].first->GetComponent<Transform>().scale.y -= 2.5f;
			}
			popList[i].second++;
			if (popList[i].second > 100)
			{
				pop = popList[i].first;
				popList.erase(popList.begin()+i);
			}

		}
		else
			popList.erase(popList.begin() + i);
	}
}
/*!*****************************************************************************
\brief
	This Function moves the selected entity basded on mouse input

\return
None.
*******************************************************************************/
void WorldViewPanel::MoveSelectedEntity()
{
	Math::Vec2 scale = e.GetComponent<Transform>().scale;
	Math::Vec2 translation = e.GetComponent<Transform>().translation;
	Math::Vec2 distance = camMousePos - translation;
	if (Input::CheckKey(E_STATE::HOLD, E_KEY::M_BUTTON_L) && (abs(distance.x) < abs(scale.x) / 2 && abs(distance.y) < abs(scale.y) / 2))
	{
		e.GetComponent<Transform>().translation = camMousePos - objectOffset;
		isSelected = 1;
	}
	if (Input::CheckKey(E_STATE::HOLD, E_KEY::M_BUTTON_L) && isSelected == 1)
	{
		e.GetComponent<Transform>().translation.x = ((int)((camMousePos.x - objectOffset.x)/10))*10.f;
		e.GetComponent<Transform>().translation.y = ((int)((camMousePos.y - objectOffset.y) / 10)) * 10.f;
	}
	
}

void WorldViewPanel::pointPicker()
{
	MouseClickMoveCam();
}
/*!*****************************************************************************
\brief
	This Function moves the selected entity basded on mouse input using ImGuizmo

\return
None.
*******************************************************************************/
void WorldViewPanel::UseGuizmo()
{
	
	ImGuizmo::SetOrthographic(true);
	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(ImGui::GetWindowPos().x + viewportPos.x, ImGui::GetWindowPos().y + viewportPos.y,
		viewportSize.x, viewportSize.y);
	const float identity[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
	std::vector<float> trf = renderManager->GetImGuizmoMat4(e);
	float translate[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
	for (int i = 0; i < 16; ++i)
	{
		translate[i] = trf[i];
	}

	std::vector<float> cam = renderManager->GetWorldCamera().GetImGuizmoWorldToCam();
	float camera[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
	for (int i = 0; i < 16; ++i)
	{
		camera[i] = cam[i];
	}

	ImGuizmo::OPERATION opp{};
	if (SRT == 1)
	{
		opp = ImGuizmo::OPERATION::SCALE;
	}
	else if (SRT == 2)
	{
		opp = ImGuizmo::OPERATION::ROTATE;
		//std::cout << translate[0] << " : " << translate[1] << "\n";
	}
	else if (SRT == 3)
	{
		opp = ImGuizmo::OPERATION::TRANSLATE;
	}
	ImGuizmo::Manipulate(&camera[0], &identity[0], opp, ImGuizmo::LOCAL, &translate[0]);
	if (ImGuizmo::IsUsing())
	{
		if (SRT == 1)
		{
			Math::Vec2 scaleX = { translate[0] , translate[1] };
			Math::Vec2 scaleY = { translate[4] , translate[5] };
			Math::Vec2 scale = { scaleX.Magnitude() * (float)*mWindowWidth , scaleY.Magnitude() * (float)*mWindowHeight };
			e.GetComponent<Transform>().scale = scale;
		}
		else if (SRT == 2)
		{
			Math::Vec2 scale = { translate[0] , translate[1] };
			float rotation = (float)(acosf(scale.x / scale.Magnitude()));
			if (scale.y < 0.f)
			{
				rotation = -rotation;
			}
			/*float theta1 = atan2(translate[6], translate[10]);
			float c2 = scale.Magnitude();
			float theta2 = atan2(-translate[2], c2);
			float s1 = sin(theta1);
			float c1 = cos(theta1);
			float rotation = atan2(s1 * translate[8] - c1 * translate[4], c1 * translate[5] - s1 * translate[9]);*/

			/*rot.Normalize();
			float rotation{};
			if (rot.x == 0)
			{
				if (rot.y > 0)
					rotation = (float)Math::PI / 2;
				else if (rot.y < 0)
					rotation = 3 * (float)Math::PI / 2;
				else
					rotation = 0;
			}
			else
			{
				if (rot.y >= 0)
					rotation = atan2(rot.y, rot.x);
				else if (rot.y < 0)
					rotation = 2 * (float)Math::PI + atan2(rot.y, rot.x);
			}*/
			e.GetComponent<Transform>().rotation = rotation;
		}
		else if (SRT == 3)
		{
			Math::Vec2 translation = { translate[12] * (float)(*mWindowWidth / 2), translate[13] * (float)(*mWindowHeight / 2) };
			e.GetComponent<Transform>().translation = translation;
		}
	}
}
