/*!*****************************************************************************
/*!*****************************************************************************
\file HierarchyPanel.cpp
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Group: Memory Leak Studios
\date 20-10-2022
\brief
This file contains function definations for a Hierarchy Panel Editor system 
that lists the entities and its components in the scene
*******************************************************************************/
#include "HierarchyPanel.h"
#include <ECSManager.h>
#include "GameStateManager.h"
#include <RenderProps.h>

/*!*****************************************************************************
\brief
	Initializes the HierarchyPanel editor

\return
None.
*******************************************************************************/
void HierarchyPanel::Init()
{
	tag = { "PLAYER","PASSENGER", "ENEMY", "BUILDING","BACKGROUND", "ENVIRONMENT","EFFECTS","PREFABS","OTHERS" };
	showDetails = false;
}
/*!*****************************************************************************
\brief
	Updates the Hierarchy Panel editor

\return
None.
*******************************************************************************/
void HierarchyPanel::Update()
{
	static int hack = 1;
	if (ImGui::Begin("Hierarchy Manager"))
	{
		int id = 0;
		ImGui::Text("GameState Selection:");
		if (ImGui::BeginTabBar("GameState"))
		{
			for (int g = 0; g < (*mGameStates).size(); g++)
			{
				if (ImGui::BeginTabItem((*mGameStates)[g].mName.c_str()))
				{
					static std::string gsName = (*mGameStates)[g].mName;
					if (selectedGameState != g && isScenePaused)
					{
						SceneReset();
						gsName = (*mGameStates)[g].mName;
						GameStateManager::GetInstance()->SetGameState((*mGameStates)[g].mName);
						selectedGameState = g;
					}
					ImGui::InputText("GameState Name", &gsName);
					if (ImGui::IsItemDeactivatedAfterEdit())
					{
						GameStateManager::GetInstance()->RenameGameState(&(*mGameStates)[g], gsName);
					}
					std::string saveGSbtn = "SAVE GameState";
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.f, 0.5f, 0.f, 1.0f });
					if (ImGui::Button(saveGSbtn.c_str()))
					{
						(*mGameStates)[g].Save();
						SceneReset();
					}
					ImGui::PopStyleColor();
					ImGui::SameLine(0.f, 1.f);
					ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 140.f, ImGui::GetCursorPos().y));
					std::string removeGSbtn = "REMOVE GameState";
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.f, 0.f, 1.0f });
					if (ImGui::Button(removeGSbtn.c_str()))
					{
						SetScenePaused(true);
						GameStateManager::GetInstance()->RemoveGameState(&(*mGameStates)[g]);
						selectedScene = 0;
						selectedGameState = 0;
						SceneReset();
						ImGui::EndTabItem();
						ImGui::PopStyleColor();
						break;
					}
					ImGui::PopStyleColor();
					//free camera
					{
						//cam pos
						float pos[2] = { (*mGameStates)[g].mCamera.translation.x , (*mGameStates)[g].mCamera.translation.y };
						ImGui::DragFloat2("Camera Pos", pos);
						(*mGameStates)[g].mCamera.translation = Math::Vec2{ pos[0], pos[1] };
						//cam size
						float size[2] = { (*mGameStates)[g].mCamera.scale.x, (*mGameStates)[g].mCamera.scale.y };
						ImGui::DragFloat("Camera Width", &size[0], 1.f, 0.f);

						//set camera
						if (ImGui::IsItemActive())
						{
							(*mGameStates)[g].mCamera.scale.x = size[0];
						}
						ImGui::DragFloat("Camera Height", &size[1], 1.f, 0.f);
						if (ImGui::IsItemActive())
						{
							(*mGameStates)[g].mCamera.scale.y = size[1];
						}
						//zoom
						float zoom = (*mGameStates)[g].mCamera.rotation;
						ImGui::DragFloat("Camera Zoom", &zoom, 0.005f);
						if (ImGui::IsItemActive())
						{
							(*mGameStates)[g].mCamera.rotation = zoom;
						}
						(*mGameStates)[g].mCamera.rotation = zoom;
					}
					ImGui::Text("Scenes Selection:");
					if (ImGui::BeginTabBar("Scenes"), ImGuiTabBarFlags_Reorderable)
					{
						if (ImGui::BeginTabItem("All"))
						{
							if (selectedScene != 99)
							{
								SceneReset();
								for (int s = 0; s < (*mGameStates)[g].mScenes.size(); s++)
									for (const Entity& e : (*mGameStates)[g].mScenes[s].mEntities)//REMOVEME aft jazz
										e.GetComponent<General>().isPaused = (*mGameStates)[g].mScenes[s].mIsPause;
								selectedScene = 99;
							}
							ImGui::Checkbox("showdetails ", &showDetails);
							for (int s = 0; s < (*mGameStates)[g].mScenes.size(); s++)
							{

								if (ImGui::CollapsingHeader((*mGameStates)[g].mScenes[s].mName.c_str()))
								{
									for (const Entity& e : (*mGameStates)[g].mScenes[s].mEntities)
									{
										ImGui::PushID(id++);
										ImGui::Text(e.GetComponent<General>().name.c_str());
										ImGui::PopID();
									}
								}
								if(showDetails)
								{
									bool isPause = !(*mGameStates)[g].mScenes[s].mIsPause;
									bool old = isPause;
									std::string showScenebtn = "Show " + (*mGameStates)[g].mScenes[s].mName + " Scene";
									ImGui::PushID(id++);
									ImGui::Checkbox("Show ", &isPause);
									ImGui::PopID();
									if (isPause != old)
										(*mGameStates)[g].mScenes[s].Pause(!isPause);
									ImGui::SameLine();
									ImGui::PushID(id++);
									ImGui::InputInt("Layer", &((*mGameStates)[g].mScenes[s].mLayer));
									ImGui::PopID();
								}
							}
							ImGui::EndTabItem(); //end ALL scene item
						}

						for (int s = 0; s < (*mGameStates)[g].mScenes.size(); s++)
						{
							if (ImGui::BeginTabItem((*mGameStates)[g].mScenes[s].mName.c_str()))
							{
								if (selectedScene != s)
								{
									if (hack)
									for (const Entity& e : *myEntities)//REMOVEME aft jazz
										e.GetComponent<General>().isPaused = true;
									
									SceneReset();
									selectedScene = s;

									if (hack)
									for (const Entity& e : (*mGameStates)[g].mScenes[s].mEntities)//REMOVEME aft jazz
										e.GetComponent<General>().isPaused = false;
									
								}
								std::string sName = (*mGameStates)[g].mScenes[s].mName;
								ImGui::InputText("Scene Name", &sName);
								if (ImGui::IsItemDeactivatedAfterEdit())
								{
										(*mGameStates)[g].mScenes[s].mName = sName;
								}
								std::string saveScenebtn = "SAVE Scene";
								ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.f, 0.5f, 0.f, 1.0f });
								if (ImGui::Button(saveScenebtn.c_str()))
								{
									(*mGameStates)[g].mScenes[s].Save();
									SceneReset();
								}
								ImGui::PopStyleColor();
								ImGui::SameLine(0.f, 1.f);
								ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth()-110.f, ImGui::GetCursorPos().y));
								std::string removeScenebtn = "REMOVE Scene";
								ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.8f, 0.f, 0.f, 1.0f });
								if (ImGui::Button(removeScenebtn.c_str()))
								{
									SetScenePaused(true);
									(*mGameStates)[g].RemoveScene((*mGameStates)[g].mScenes[s].mName);
									selectedScene = 0;
									SceneReset();
									ImGui::EndTabItem();
									ImGui::PopStyleColor();
									break;
								}
								ImGui::PopStyleColor();

								/// Game Camera Editor
								if (ImGui::CollapsingHeader("Scene Camera")||true)
								{
									bool isPause = !(*mGameStates)[g].mScenes[s].mIsPause;
									bool old = isPause;
									ImGui::Checkbox("Show Scene", &isPause);
									if(isPause != old)
										(*mGameStates)[g].mScenes[s].Pause(!isPause);
									ImGui::Checkbox("Fix Camera for UI", &(*mGameStates)[g].mScenes[s].mIsUI);
									ImGui::Checkbox("mForceRender", &(*mGameStates)[g].mScenes[s].mForceRender);
									ImGui::InputInt("Layer", &((*mGameStates)[g].mScenes[s].mLayer));
									if (ImGui::IsItemDeactivatedAfterEdit())
									{
										(*mGameStates)[g].mScenes[s].mLayer = (*mGameStates)[g].mScenes[s].mLayer < 0 ? 0
											: ((*mGameStates)[g].mScenes[s].mLayer > MAX_SCENE_LAYERS -1  ? MAX_SCENE_LAYERS -1
												: (*mGameStates)[g].mScenes[s].mLayer);
									}
									ImGui::InputInt("Order", &((*mGameStates)[g].mScenes[s].mOrder));
									if (ImGui::IsItemDeactivatedAfterEdit())
									{
										(*mGameStates)[g].mScenes[s].mOrder = (*mGameStates)[g].mScenes[s].mOrder < 0 ? 0
											: ((*mGameStates)[g].mScenes[s].mOrder > (int)(*mGameStates)[g].mScenes.size() ? (int)(*mGameStates)[g].mScenes.size()
												: (*mGameStates)[g].mScenes[s].mOrder);
									}

								}
								for (int i = 0; i < (int)tag.size(); i++)
								{

									if (ImGui::CollapsingHeader(tag[i].c_str()))
									{
										for (int l = -1; l < 1024; l++)
										{
											for (const Entity& e : (*mGameStates)[g].mScenes[s].mEntities)
											{
												if (e.GetComponent<General>().tag != (TAG)i)
													continue;
											
												if (l == -1)
												{
													if (!e.HasComponent<Sprite>())
													{
														ImGui::PushID(id++);
														listComponents(&e, e.GetComponent<General>().name);
														ImGui::PopID();
													}
												}
												else
												{
													if (!e.HasComponent<Sprite>())
														continue;
													if (e.GetComponent<Sprite>().layer != l)
														continue;
													std::string name = "(" + std::to_string(l) + ") " + e.GetComponent<General>().name;
													ImGui::PushID(id++);
													listComponents(&e, name);
													ImGui::PopID();
												}
											}
										}
									}
								}
								if (ImGui::BeginPopupContextWindow(0, 1, false))
								{
									if (ImGui::MenuItem("Create Entity"))
									{
										NewEntity();
									}
									ImGui::EndPopup();
								}
								ImGui::NewLine();
								if (ImGui::Button("NEW Entity"))
								{
									NewEntity();
								}
								ImGui::EndTabItem(); //end scene item

							}
							//ImGui::PopID();
						}
						ImGui::EndTabBar(); //for scene
					}
					std::string newScenebtn = "NEW Scene";
					if (ImGui::Button(newScenebtn.c_str()))
					{//FUNCTION GS SCENE
						NewScene();
					}
					ImGui::EndTabItem(); //forGS
				}
				//else
					//ImGui::PopID();
			}
			ImGui::EndTabBar();
		}

		if (ImGui::Button("NEW GameState"))
		{//FUNCTION GS SCENE
			NewGameState();
		}

	}
	ImGui::End();
}
/*!*****************************************************************************
\brief
	Free the Hierarchy Panel editor

\return
None.
*******************************************************************************/
void HierarchyPanel::Free()
{

}
/*!*****************************************************************************
\brief
	This function creates a new entity

\return
None.
*******************************************************************************/
//void HierarchyPanel::newEntity()
//{
//	LOG_INFO("Created new entity");
//	Math::Vec2 campos = renderManager->GetWorldCamera().GetPos();
//
//	/*Entity e{ ECS::CreateEntity() };
//	//(allEntities[selectedGameState][selectedScene]).insert(e);
//	(*mGameStates)[selectedGameState].mScenes[selectedScene].mEntities.insert(e);*/
//
//	Entity e = (*mGameStates)[selectedGameState].mScenes[selectedScene].AddEntity();
//
//	e.AddComponent(
//		General{ "NEW Entity " + std::to_string(newEntityCount), TAG::OTHERS, SUBTAG::NOSUBTAG, true , false },
//		Transform{ {150,150}, 0, campos },
//		Sprite{ Color{0,255,0,255}, SPRITE::CIRCLE, 0,highestLayer });
//	newEntityCount++;
//}

/*!*****************************************************************************
\brief
	This function sets the selected entity

\return
None.
*******************************************************************************/

void HierarchyPanel::setSelectedEntity(const Entity* e)
{
	if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		selectedEntity = e;
		selectedPrefab = nullptr;
		selectedType = 1;
		aspect = false;
		renderManager->SelectEntity(*selectedEntity);
	}
}

/*!*****************************************************************************
\brief
	This function sets the selected entity or prefab

\return
None.
*******************************************************************************/
//void HierarchyPanel::setSelected(Entity* e)
//{
//	if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
//	{
//		selectedEntity = e;
//		selectedPrefab = nullptr;
//		selectedType = 1;
//		
//	}
//}


/*!*****************************************************************************
\brief
	This function list components for entities or prefabs

\return
None.
*******************************************************************************/

void HierarchyPanel::listComponents(const Entity* e, std::string _name)
{
	
	if (ImGui::TreeNode(_name.c_str()))
	{
		if (e->HasComponent<General>())
		{
			ImGui::Text("General");
			setSelectedEntity(e);
		}
		if (e->HasComponent<Lifespan>())
		{
			ImGui::Text("Lifespan");
			setSelectedEntity(e);
		}
		if (e->HasComponent<Transform>())
		{
			ImGui::Text("Transform");
			setSelectedEntity(e);
		}
		if (e->HasComponent<Sprite>())
		{
			ImGui::Text("Sprite");
			setSelectedEntity(e);
		}
		if (e->HasComponent<Animation>())
		{
			ImGui::Text("Animation");
			setSelectedEntity(e);
		}
		if (e->HasComponent<SheetAnimation>())
		{
			ImGui::Text("SheetAnimation");
			setSelectedEntity(e);
		}
		if (e->HasComponent<Physics2D>())
		{
			ImGui::Text("Physics2D");
			setSelectedEntity(e);
		}
		if (e->HasComponent<RectCollider>())
		{
			ImGui::Text("RectCollider");
			setSelectedEntity(e);
		}
		if (e->HasComponent<CircleCollider>())
		{
			ImGui::Text("CircleCollider");
			setSelectedEntity(e);
		}
		if (e->HasComponent<Edge2DCollider>())
		{
			ImGui::Text("Edge2DCollider");
			setSelectedEntity(e);
		}
		if (e->HasComponent<Point2DCollider>())
		{
			ImGui::Text("Point2DCollider");
			setSelectedEntity(e);
		}
		if (e->HasComponent<LayerCollider>())
		{
			ImGui::Text("LayerCollider");
			setSelectedEntity(e);
		}
		if (e->HasComponent<Audio>())
		{
			ImGui::Text("Audio");
			setSelectedEntity(e);
		}
		if (e->HasComponent<Text>())
		{
			ImGui::Text("Text");
			setSelectedEntity(e);
		}
		if (e->HasComponent<AI>())
		{
			ImGui::Text("AI");
			setSelectedEntity(e);
		}
		if (e->HasComponent<Script>())
		{
			ImGui::Text("Script");
			setSelectedEntity(e);
		}
		if (e->HasComponent<Button>())
		{
			ImGui::Text("Button");
			setSelectedEntity(e);
		}
		if (e->HasComponent<Dialogue>())
		{
			ImGui::Text("Dialogue");
			setSelectedEntity(e);
		}
		if (e->HasComponent<LightSource>())
		{
			ImGui::Text("LightSource");
			setSelectedEntity(e);
		}
		if (e->HasComponent<ShadowCaster>())
		{
			ImGui::Text("ShadowCaster");
			setSelectedEntity(e);
		}
		if (e->HasComponent<Viewport>())
		{
			ImGui::Text("Viewport");
			setSelectedEntity(e);
		}
		if (e->HasComponent<MovementAI>())
		{
			ImGui::Text("MovementAI");
			setSelectedEntity(e);
		}
		if (e->HasComponent<ColorAI>())
		{
			ImGui::Text("ColorAI");
			setSelectedEntity(e);
		}
		if (e->HasComponent<ParticleSystem>())
		{
			ImGui::Text("ParticleSystem");
			setSelectedEntity(e);
		}
		ImGui::TreePop();
	}
	setSelectedEntity(e);
}

