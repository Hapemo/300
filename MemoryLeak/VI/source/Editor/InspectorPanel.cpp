/*!*****************************************************************************
/*!*****************************************************************************
\file LevelEditor.cpp
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Group: Memory Leak Studios
\date 20-09-2022
\brief
This file contains function definations for a Inspector Panel Editor system that modifies
Entities and its Components.
*******************************************************************************/
#include "InspectorPanel.h"
#include <ECSManager.h>
#include "ScriptManager.h"
#include "GameStateManager.h"
#include "RenderProps.h"
/*!*****************************************************************************
\brief
	Initializes the Inspector Panel editor

\return
None.
*******************************************************************************/
void InspectorPanel::Init()
{

}
/*!*****************************************************************************
\brief
	Update the Inspector Panel editor

\return
None.
*******************************************************************************/
void InspectorPanel::Update()
{
	if (ImGui::Begin("Inspector Manager"))
	{
		ImGui::BeginTabBar("Inspector ");
		if (ImGui::BeginTabItem("Edit Game: "))
		{
			if (selectedEntity != nullptr)
			{
				e = *selectedEntity;

				if (e.HasComponent<General>())
				{
					GeneralEditor();
				}
				if (e.HasComponent<Lifespan>())
				{
					LifespanEditor();
				}
				if (e.HasComponent<Transform>())
				{
					TransformEditor();
				}
				if (e.HasComponent<Sprite>())
				{
					SpriteEditor();
				}
				if (e.HasComponent<Animation>())
				{
					AnimationEditor();
				}
				if (e.HasComponent<SheetAnimation>())
				{
					SheetAnimationEditor();
				}
				if (e.HasComponent<Physics2D>())
				{
					Physics2DEditor();
				}
				if (e.HasComponent<RectCollider>())
				{
					RectColliderEditor();
				}
				if (e.HasComponent<CircleCollider>())
				{
					CircleColliderEditor();
				}
				if (e.HasComponent<Edge2DCollider>())
				{
					Edge2DColliderEditor();
				}
				if (e.HasComponent<Point2DCollider>())
				{
					Point2DColliderEditor();
				}
				if (e.HasComponent<LayerCollider>())
				{
					LayerColliderEditor();
				}
				if (e.HasComponent<Audio>())
				{
					AudioEditor();
				}
				if (e.HasComponent<Text>())
				{
					TextEditor();
				}
				if (e.HasComponent<AI>())
				{
					AIEditor();
				}
				if (e.HasComponent<Script>())
				{
					ScriptEditor();
				}
				if (e.HasComponent<Dialogue>())
				{
					DialogueEditor();
				}
				if (e.HasComponent<Button>())
				{
					ButtonEditor();
				}
				if (e.HasComponent<LightSource>())
				{
					LightSourceEditor();
				}
				if (e.HasComponent<ShadowCaster>())
				{
					ShadowCasterEditor();
				}
				if (e.HasComponent<Viewport>())
				{
					ViewportEditor();
				}
				if (e.HasComponent<MovementAI>())
				{
					MovementAIEditor();
				}
				if (e.HasComponent<ColorAI>())
				{
					ColorAIEditor();
				}
				if (e.HasComponent<ParticleSystem>())
				{
					ParticleSystemEditor();
				}
				ImGui::Combo("Select Component", &addComponentID, componentsList, IM_ARRAYSIZE(componentsList));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.f, 0.5f, 0.f, 1.0f });
				if (ImGui::Button("Add Component"))
				{
					AddComponent();
					std::string add(componentsList[addComponentID]);
					LOG_INFO(add + " conponent added");
				}
				ImGui::PopStyleColor();
				ImGui::NewLine();
				ImGui::Separator();
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.7f, 0.f, 0.f, 1.0f });
				if (ImGui::Button("Cut   (CTRL+X)"))
				{
					Cut();
				}
				ImGui::PopStyleColor();
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.0f, 0.f, 0.6f, 1.0f });
				if (ImGui::Button("Copy  (CTRL+C)"))
				{
					Copy();
				}
				ImGui::PopStyleColor();
				if (copyEntity.second != 0)
				{
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.f, 0.5f, 0.f, 1.0f });
					if (ImGui::Button("Paste (CTRL+V)"))
					{
						Paste();
					}
					ImGui::PopStyleColor();
				}
				ImGui::Separator();
				if (stackPointer > 0)
				{
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.5f, 1.0f });
					if (ImGui::Button("Undo  (CTRL+Z)"))
					{
						Undo();
					}
					ImGui::PopStyleColor();
				}
				if (stackPointer + 1 < (int)undoStack.size())
				{
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.f, 0.6f, 0.7f, 1.0f });
					if (ImGui::Button("Redo  (CTRL+Y)"))
					{
						Redo();
					}
					ImGui::PopStyleColor();
				}
				ImGui::Separator();

				ImGui::NewLine();
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.f, 0.f, 1.0f });
				if (ImGui::Button("Delete Entity"))
				{
					DeleteEntity();
				}
				ImGui::PopStyleColor();
				if (ImGui::BeginPopupContextWindow(0, 1, false))
				{
					if (ImGui::MenuItem("Delete Entity"))
					{
						DeleteEntity();
					}
					ImGui::EndPopup();
				}
			}
			else if (selectedPrefab != nullptr)
			{
				p = selectedPrefab;
				PrefabEditor();
				ImGui::Combo("Select Prefab Component", &addComponentID, componentsList, IM_ARRAYSIZE(componentsList));
				if (ImGui::Button("Add Component to prefab"))
				{
					AddPrefabComponent();
					std::string add(componentsList[addComponentID]);
					LOG_INFO(add + " conponent added to prefab");
				}
			}
			else
			{
				ImGui::Text("Select an Entity");
				if (copyEntity.second != 0)
				{
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.f, 0.5f, 0.f, 1.0f });
					if (ImGui::Button("Paste (CTRL+V)"))
					{
						Paste();
					}
					ImGui::PopStyleColor();
					ImGui::Separator();
				}
				/*if (stackPointer > 0)
				{
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.5f, 1.0f });
					if (ImGui::Button("Undo  (CTRL+Z)"))
					{
						Undo();
					}
					ImGui::PopStyleColor();
				}
				if (stackPointer+1 < (int)undoStack.size())
				{
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.f, 0.6f, 0.7f, 1.0f });
					if (ImGui::Button("Redo  (CTRL+Y)"))
					{
						Redo();
					}
					ImGui::PopStyleColor();
				}*/
			}
			ImGui::EndTabItem();
		}
		if (Input::CheckKey(E_STATE::PRESS, E_KEY::DELETE))
		{
			DeleteEntity();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
}
/*!*****************************************************************************
\brief
	Initializes the Inspector Panel editor

\return
None.
*******************************************************************************/
void InspectorPanel::Free()
{

}
/*!*****************************************************************************
\brief
	This function add a component to an entity

\return
None.
*******************************************************************************/
void InspectorPanel::AddComponent()
{
	if (addComponentID == (int)COMPONENTID::GENERAL)
		e.AddComponent<General>({ "_new_", (TAG)0, (SUBTAG)0, true , false});
	else if (addComponentID == (int)COMPONENTID::LIFESPAN)
		e.AddComponent<Lifespan>({ 0,1000 });
	else if (addComponentID == (int)COMPONENTID::TRANSFORM)
		e.AddComponent<Transform>({ {100,100}, 0, {0,0} });
	else if (addComponentID == (int)COMPONENTID::SPRITE)
		e.AddComponent<Sprite>({});
	else if (addComponentID == (int)COMPONENTID::ANIMATION)
	{
		e.AddComponent<Animation>({});
		if (e.HasComponent<Sprite>())
			animator->AddImages(e, SpriteSheet{e.GetComponent<Sprite>(safe).texture});
	}
	else if (addComponentID == (int)COMPONENTID::SHEETANIMATION)
		e.AddComponent<SheetAnimation>({});
	else if (addComponentID == (int)COMPONENTID::PHYSICS2D)
		e.AddComponent<Physics2D>({});
	else if (addComponentID == (int)COMPONENTID::RECTCOLLIDER)
		e.AddComponent<RectCollider>({ });
	else if (addComponentID == (int)COMPONENTID::CIRCLECOLLIDER)
		e.AddComponent<CircleCollider>({});
	else if (addComponentID == (int)COMPONENTID::EDGE2DCOLLIDER)
		e.AddComponent<Edge2DCollider>({});
	else if (addComponentID == (int)COMPONENTID::POINT2DCOLLIDER)
		e.AddComponent<Point2DCollider>({});
	else if (addComponentID == (int)COMPONENTID::AUDIO)
		e.AddComponent<Audio>({});
	else if (addComponentID == (int)COMPONENTID::TEXT)
		e.AddComponent<Text>({ "BPtypewrite", "PLAY", Math::Vec2{0,0}, 1.f, Color{ 255,255,255,255 } });
	else if (addComponentID == (int)COMPONENTID::AI)
		e.AddComponent<AI>({});
	else if (addComponentID == (int)COMPONENTID::SCRIPT)
		e.AddComponent<Script>({});
	else if (addComponentID == (int)COMPONENTID::DIALOGUE)
		e.AddComponent<Dialogue>({});
	else if (addComponentID == (int)COMPONENTID::BUTTON)
		e.AddComponent<Button>({});
	else if (addComponentID == (int)COMPONENTID::LAYERCOLLIDER)
		e.AddComponent<LayerCollider>({});
	else if (addComponentID == (int)COMPONENTID::LIGHTSOURCE)
		e.AddComponent<LightSource>({});
	else if (addComponentID == (int)COMPONENTID::SHADOWCASTER)
		e.AddComponent<ShadowCaster>({});
	else if (addComponentID == (int)COMPONENTID::Viewport)
		e.AddComponent<Viewport>({});
	else if (addComponentID == (int)COMPONENTID::MOVEMENTAI)
	{
		e.AddComponent<MovementAI>({});
		if (e.HasComponent<Transform>())
			movementAIManager->AddTransform(e, e.GetComponent<Transform>());
	}
	else if (addComponentID == (int)COMPONENTID::COLORAI)
	{
		e.AddComponent<ColorAI>({});
		if(e.HasComponent<Sprite>())
			colorAIManager->AddColor(e, e.GetComponent<Sprite>().color);
	}
	else if (addComponentID == (int)COMPONENTID::PARTICLESYSTEM)
		e.AddComponent<ParticleSystem>({});
	
	
}
/*!*****************************************************************************
\brief
	This function add a component to an prefab

\return
None.
*******************************************************************************/
void InspectorPanel::AddPrefabComponent()
{
	if (p == nullptr)
		return;
	if (addComponentID == (int)COMPONENTID::GENERAL)
		p->UpdateComponent(General( "_new_", (TAG)7, (SUBTAG)0, true ));
		//p->AddComponent<General>({ "_new_", (TAG)0, (SUBTAG)0, true }); ///casuses memory leaks
	else if (addComponentID == (int)COMPONENTID::LIFESPAN)
		p->AddComponent<Lifespan>({ 0,1000 });
	else if (addComponentID == (int)COMPONENTID::TRANSFORM)
		p->AddComponent<Transform>({ {100,100}, 0, {0,0} });
	else if (addComponentID == (int)COMPONENTID::SPRITE)
		p->AddComponent<Sprite>({});
	else if (addComponentID == (int)COMPONENTID::ANIMATION)
		p->AddComponent<Animation>({});
	else if (addComponentID == (int)COMPONENTID::SHEETANIMATION)
		p->AddComponent<SheetAnimation>({});
	else if (addComponentID == (int)COMPONENTID::PHYSICS2D)
		p->AddComponent<Physics2D>({});
	else if (addComponentID == (int)COMPONENTID::RECTCOLLIDER)
		p->AddComponent<RectCollider>({ });
	else if (addComponentID == (int)COMPONENTID::CIRCLECOLLIDER)
		p->AddComponent<CircleCollider>({});
	else if (addComponentID == (int)COMPONENTID::EDGE2DCOLLIDER)
		p->AddComponent<Edge2DCollider>({});
	else if (addComponentID == (int)COMPONENTID::POINT2DCOLLIDER)
		p->AddComponent<Point2DCollider>({});
	else if (addComponentID == (int)COMPONENTID::AUDIO)
		p->AddComponent<Audio>({});
	else if (addComponentID == (int)COMPONENTID::TEXT)
		p->AddComponent<Text>({ "BPtypewrite", "PLAY", Math::Vec2{0,0}, 1.f, Color{ 255,255,255,255 } });
	else if (addComponentID == (int)COMPONENTID::AI)
		p->AddComponent<AI>({});
	else if (addComponentID == (int)COMPONENTID::SCRIPT)
		p->AddComponent<Script>({});
	else if (addComponentID == (int)COMPONENTID::DIALOGUE)
		p->AddComponent<Dialogue>({});
	else if (addComponentID == (int)COMPONENTID::BUTTON)
		p->AddComponent<Button>({});
	else if (addComponentID == (int)COMPONENTID::LAYERCOLLIDER)
		p->AddComponent<LayerCollider>({});
	else if (addComponentID == (int)COMPONENTID::LIGHTSOURCE)
		p->AddComponent<LightSource>({});
	else if (addComponentID == (int)COMPONENTID::SHADOWCASTER)
		p->AddComponent<ShadowCaster>({});
	else if (addComponentID == (int)COMPONENTID::Viewport)
		p->AddComponent<Viewport>({});
	else if (addComponentID == (int)COMPONENTID::MOVEMENTAI)
	{
		p->AddComponent<MovementAI>({});
		//p->GetComponent<MovementAI>().targetTransforms.push_back(p->GetComponent<Transform>());
		//p->GetComponent<MovementAI>().time.push_back(1.f);
	}
}


/*!*****************************************************************************
\brief
	This functions below onawards edits a particular component

\return
None.
*******************************************************************************/
void InspectorPanel::GeneralEditor()
{
	if (ImGui::CollapsingHeader("General") || true) {
		ImGui::Text("General");

		ImGui::Checkbox("isActive", &e.GetComponent<General>().isActive); //isactive
		SaveUndo(e, tempComponent, COMPONENTID::GENERAL);

		ImGui::InputText("Name", &e.GetComponent<General>().name);
		SaveUndo(e, tempComponent, COMPONENTID::GENERAL);

		int tagID = (int)e.GetComponent<General>().tag;
		//static const char* tag[]{ "PLAYER","PASSENGER", "ENEMY", "BUILDING","BACKGROUND", "ENVIRONMENT","EFFECTS","PREFABS","OTHERS" };
		ImGui::Combo("Tag", &tagID, tag, IM_ARRAYSIZE(tag));
		e.GetComponent<General>().tag = (TAG)tagID;
		SaveUndo(e, tempComponent, COMPONENTID::GENERAL);

		int subtagID = (int)e.GetComponent<General>().subtag;
		//static const char* subtag[]{ "NOSUBTAG", "PLAYER", "PASSENGER", "ENEMY", "BUILDING", "OTHERS" };
		ImGui::Combo("SubTag", &subtagID, subtag, IM_ARRAYSIZE(subtag));
		e.GetComponent<General>().subtag = (SUBTAG)subtagID;
		SaveUndo(e, tempComponent, COMPONENTID::GENERAL);

	}
}
void InspectorPanel::LifespanEditor()
{
	if (ImGui::CollapsingHeader("Lifespan")) 
	{
		ImGui::InputFloat("Lifespan Limit", &e.GetComponent<Lifespan>().limit);
		SaveUndo(e, tempComponent, COMPONENTID::LIFESPAN);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.7f, 0.f, 0.f, 1.0f });
		if (ImGui::Button("Remove Lifespan"))
		{
			e.RemoveComponent<Lifespan>();
			LOG_INFO("Lifespan component removed");
		}
		ImGui::PopStyleColor();
	}
}
void InspectorPanel::TransformEditor()
{
	if (ImGui::CollapsingHeader("Transform") || true) {
		//ImGui::Text("Transform Component");
		static bool g = false;
		ImGui::Checkbox("Gizmo", &g);
		if (g)
			SRT = 4;
		else
			SRT = 0;
		if (ImGui::Button("Flip X", ImVec2(ImGui::GetWindowWidth()*0.32f, 18)))
			e.GetComponent<Transform>().scale.x *= -1.f;
		SaveUndo(e, tempComponent, COMPONENTID::TRANSFORM);
		ImGui::SameLine(0.f, 5.f);
		if (ImGui::Button("Flip Y", ImVec2(ImGui::GetWindowWidth() * 0.32f, 18)))
			e.GetComponent<Transform>().scale.y *= -1.f;
		SaveUndo(e, tempComponent, COMPONENTID::TRANSFORM);
		ImGui::SameLine(0.f, 5.f);
		ImGui::Checkbox("Aspect ratio", &aspect);

		//scale
		tmpVec2[0] = e.GetComponent<Transform>().scale.x;
		tmpVec2[1] = e.GetComponent<Transform>().scale.y;
		SaveUndo(e, tempComponent, COMPONENTID::TRANSFORM);
		float ratio = tmpVec2[1] / tmpVec2[0];
		ImGui::DragFloat2("Set Scale", tmpVec2);
		if (aspect)
		{
			if (e.HasComponent<Sprite>() && e.GetComponent<Sprite>().sprite == SPRITE::TEXTURE)
				tmpVec2[1] = tmpVec2[0] * GET_ASPECT_RATIO(e.GetComponent<Sprite>().texture);
			else
				tmpVec2[1] = tmpVec2[0]*ratio;
		}
		e.GetComponent<Transform>().scale = Math::Vec2(tmpVec2[0], tmpVec2[1]);
		SaveUndo(e, tempComponent, COMPONENTID::TRANSFORM);


		//translate
		tmpVec2[0] = e.GetComponent<Transform>().translation.x;
		tmpVec2[1] = e.GetComponent<Transform>().translation.y;
		ImGui::DragFloat2("Set Position", tmpVec2);
		e.GetComponent<Transform>().translation = Math::Vec2(tmpVec2[0], tmpVec2[1]);
		SaveUndo(e, tempComponent, COMPONENTID::TRANSFORM);

		//rotate
		tmpFloat = e.GetComponent<Transform>().rotation;
		tmpFloat = (float)(tmpFloat / M_PI * 180.f);
		ImGui::DragFloat("Set Rotation", &tmpFloat,1.f, -360.f, 360.f);
		tmpFloat = (float)(tmpFloat * M_PI / 180.f);
		e.GetComponent<Transform>().rotation = tmpFloat;
		SaveUndo(e, tempComponent, COMPONENTID::TRANSFORM);
		/*if (ImGui::Button("Remove Transform"))
		{
			e.RemoveComponent<Transform>();
			LOG_INFO("Transform component removed");
		}*/
	}
}
void InspectorPanel::SpriteEditor()
{
	if (ImGui::CollapsingHeader("Sprite")) {
		//ImGui::Text("Sprite");
		tmpVec4[0] = e.GetComponent<Sprite>().color.r / 255.f;
		tmpVec4[1] = e.GetComponent<Sprite>().color.g / 255.f;
		tmpVec4[2] = e.GetComponent<Sprite>().color.b / 255.f;
		tmpVec4[3] = e.GetComponent<Sprite>().color.a / 255.f;
		ImGui::ColorEdit4("Sprite Color", tmpVec4);
		//ImGui::ColorPicker4("Color", tmpVec4);
		e.GetComponent<Sprite>().color.r = (GLubyte)(tmpVec4[0] * 255);
		e.GetComponent<Sprite>().color.g = (GLubyte)(tmpVec4[1] * 255);
		e.GetComponent<Sprite>().color.b = (GLubyte)(tmpVec4[2] * 255);
		e.GetComponent<Sprite>().color.a = (GLubyte)(tmpVec4[3] * 255);
		SaveUndo(e, tempComponent, COMPONENTID::SPRITE);

		std::string tex{};
		if (e.GetComponent<Sprite>().sprite == SPRITE::TEXTURE)
			tex = spriteManager->GetTexturePath(spriteManager->GetTexture(e));
		else if (e.GetComponent<Sprite>().sprite == SPRITE::CIRCLE)
			tex = "CIRCLE";
		else if (e.GetComponent<Sprite>().sprite == SPRITE::SQUARE)
			tex = "SQUARE";
		int shapeID = (int)e.GetComponent<Sprite>().sprite;
		//static const char* shape[]{ "SQUARE", "CIRCLE", "TEXTURE","DEBUG_POINT" , "DEBUG_LINE","DEBUG_SQUARE","DEBUG_CIRCLE", "DEBUG_ARROW" };
		ImGui::Combo("Shape", &shapeID, shape, IM_ARRAYSIZE(shape));
		e.GetComponent<Sprite>().sprite = (SPRITE)shapeID;
		SaveUndo(e, tempComponent, COMPONENTID::SPRITE);

		if ((SPRITE)shapeID != SPRITE::TEXTURE)
		{
			e.GetComponent<Sprite>().texture = 0;
		}
		if (e.GetComponent<Sprite>().sprite == SPRITE::TEXTURE)
		{
			ImGui::InputText("Sprite", &tex);
			spriteManager->SetTexture(e, tex);
			SaveUndo(e, tempComponent, COMPONENTID::SPRITE);

		}
		else
		{
			ImGui::Text(tex.c_str());
		}

		if (ImGui::BeginDragDropTarget())
		{
			static const wchar_t* texpath = (const wchar_t*)"";
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURES"))
			{
				texpath = (const wchar_t*)payload->Data;
				std::string tp = (std::string)((const char*)texpath);
				e.GetComponent<Sprite>().sprite = SPRITE::TEXTURE;
				spriteManager->SetTexture(e, tp);
				COMPONENT _new;
				_new = e.GetComponent<Sprite>();
				undoStack.push_back(std::make_pair(e, _new));
				stackPointer = (int)undoStack.size();
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::InputInt("Layer", &e.GetComponent<Sprite>().layer);
		if (ImGui::IsItemDeactivatedAfterEdit())
		{
			e.GetComponent<Sprite>().layer = e.GetComponent<Sprite>().layer < 0 ? 0
				: (e.GetComponent<Sprite>().layer > MAX_LAYERS_PER_SCENE - 1 ? MAX_LAYERS_PER_SCENE - 1// 63 ? 63
					: e.GetComponent<Sprite>().layer);
		}
		SaveUndo(e, tempComponent, COMPONENTID::SPRITE);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.7f, 0.f, 0.f, 1.0f });
		if (ImGui::Button("Remove Sprite"))
		{
			e.RemoveComponent<Sprite>();
			LOG_INFO("Sprite component removed");
		}
		ImGui::PopStyleColor();
	}
}
void InspectorPanel::AnimationEditor()
{
	if (ImGui::CollapsingHeader("Animation")) {
		//ImGui::Text("Animation");
		//static GLuint addImage = {};
		static SpriteSheet addImage{};
		static std::string  texadd = "Add image";
		for (size_t i = 0; i <= e.GetComponent<Animation>().sheets.size(); ++i)
		{
			std::string tex{};
			if (i != e.GetComponent<Animation>().sheets.size())
			{
				tex = spriteManager->GetTexturePath(e.GetComponent<Animation>().sheets[i].sheet);
				ImGui::InputText(("Image "+std::to_string(i)).c_str(), const_cast<char*>(tex.c_str()), texadd.size());
				if (ImGui::BeginDragDropTarget())
				{
					static const wchar_t* texpath = (const wchar_t*)"";
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURES"))
					{
						texpath = (const wchar_t*)payload->Data;
						std::string tp = (std::string)((const char*)texpath);
						e.GetComponent<Animation>().sheets[i].sheet = spriteManager->GetTextureID(tp);
						COMPONENT _new;
						_new = e.GetComponent<Animation>();
						undoStack.push_back(std::make_pair(e, _new));
						stackPointer = (int)undoStack.size();
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::InputInt(("frameCount"+std::to_string(i)).c_str(), &e.GetComponent<Animation>().sheets[i].frameCount);
				ImGui::InputFloat(("timePerFrame"+std::to_string(i)).c_str(), &e.GetComponent<Animation>().sheets[i].timePerFrame);
				if (ImGui::Button(("Remove Sheet" + std::to_string(i)).c_str()))
				{
					e.GetComponent<Animation>().sheets.erase(e.GetComponent<Animation>().sheets.begin() + i);
				}
				ImGui::Separator();
			}
			else
			{
				ImGui::InputText("Addimage", const_cast<char*>(texadd.c_str()), texadd.size());
				tex = texadd;
				static const wchar_t* texpath = (const wchar_t*)"";
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURES"))
					{
						texpath = (const wchar_t*)payload->Data;
						if (i == e.GetComponent<Animation>().sheets.size())
							texadd = (char*)texpath;
						std::string  tp = (std::string)((const char*)texpath);
						if (i != e.GetComponent<Animation>().sheets.size())
							e.GetComponent<Animation>().sheets[i].sheet = spriteManager->GetTextureID(tp);
						else
							addImage.sheet = spriteManager->GetTextureID(tp);
					}
					ImGui::EndDragDropTarget();
				}
			}
			
			SaveUndo(e, tempComponent, COMPONENTID::ANIMATION);
		}
		if (ImGui::Button("Add Sprite"))
		{
			animator->AddImages(e, addImage); //yj
		}
		//SaveUndo(e, tempComponent, COMPONENTID::ANIMATION);
		//ImGui::InputFloat("timePerImage", &e.GetComponent<Animation>().timePerImage[e.GetComponent<Animation>().currentImageIndex]); //yj
		//SaveUndo(e, tempComponent, COMPONENTID::ANIMATION);
		//ImGui::InputFloat("timeToImageSwap", &e.GetComponent<Animation>().timeToImageSwap);
		//SaveUndo(e, tempComponent, COMPONENTID::ANIMATION);
		ImGui::InputInt("currentImageIndex", &e.GetComponent<Animation>().currentImageIndex);
		SaveUndo(e, tempComponent, COMPONENTID::ANIMATION);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.7f, 0.f, 0.f, 1.0f });
		if (ImGui::Button("Remove Animation"))
		{
			e.RemoveComponent<Animation>();
			LOG_INFO("Animation component removed");
		}
		ImGui::PopStyleColor();
	}
}
void InspectorPanel::SheetAnimationEditor()
{
	if (ImGui::CollapsingHeader("SheetAnimation")||isAnimatorEditorFocused()) {
		//ImGui::Text("SheetAnimation");
		ImGui::InputInt("frameCount", &e.GetComponent<SheetAnimation>().frameCount);
		SaveUndo(e, tempComponent, COMPONENTID::SHEETANIMATION);
		ImGui::InputInt("currFrameIndex", (int*)&e.GetComponent<SheetAnimation>().currFrameIndex);
		SaveUndo(e, tempComponent, COMPONENTID::SHEETANIMATION);
		ImGui::InputFloat("timePerFrame", &e.GetComponent<SheetAnimation>().timePerFrame);
		SaveUndo(e, tempComponent, COMPONENTID::SHEETANIMATION);
		//ImGui::InputFloat("timeToFrameSwap", &e.GetComponent<SheetAnimation>().timeToFrameSwap);
		//SaveUndo(e, tempComponent, COMPONENTID::SHEETANIMATION);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.7f, 0.f, 0.f, 1.0f });
		if (ImGui::Button("Remove SheetAnimation"))
		{
			e.RemoveComponent<SheetAnimation>();
			LOG_INFO("SheetAnimation component removed");
		}
		ImGui::PopStyleColor();
	}
}
void InspectorPanel::Physics2DEditor()
{

	if (ImGui::CollapsingHeader("Physics2D")) {
		//ImGui::Text("Physics2D");
		// 
		// 
		//ImGui::Checkbox("gravityEnabled", &e.GetComponent<Physics2D>().gravityEnabled);

		////////
		//ImGui::InputFloat("Mass", &e.GetComponent<Physics2D>().mass);
		//SaveUndo(e, tempComponent, COMPONENTID::PHYSICS2D);
		//ImGui::InputFloat("Speed", &e.GetComponent<Physics2D>().speed);
		//SaveUndo(e, tempComponent, COMPONENTID::PHYSICS2D);
		//ImGui::InputFloat("moveDirection", &e.GetComponent<Physics2D>().moveDirection);
		//SaveUndo(e, tempComponent, COMPONENTID::PHYSICS2D);
		///////
		//ImGui::InputDouble("gravityScale", &e.GetComponent<Physics2D>().gravityScale);

		ImGui::Checkbox("dynamicsEnabled", &e.GetComponent<Physics2D>().dynamicsEnabled);
		SaveUndo(e, tempComponent, COMPONENTID::PHYSICS2D);

		ImGui::DragFloat("Mass", &e.GetComponent<Physics2D>().mass, 0.1f);
		SaveUndo(e, tempComponent, COMPONENTID::PHYSICS2D);

		ImGui::DragFloat("inertia", &e.GetComponent<Physics2D>().inertia, 0.1f);
		SaveUndo(e, tempComponent, COMPONENTID::PHYSICS2D);

		ImGui::DragFloat("restitution", &e.GetComponent<Physics2D>().restitution, 0.1f);
		SaveUndo(e, tempComponent, COMPONENTID::PHYSICS2D);

		ImGui::DragFloat("friction", &e.GetComponent<Physics2D>().friction, 0.1f);
		SaveUndo(e, tempComponent, COMPONENTID::PHYSICS2D);

		ImGui::DragFloat("damping", &e.GetComponent<Physics2D>().damping, 0.1f);
		SaveUndo(e, tempComponent, COMPONENTID::PHYSICS2D);

		tmpVec2[0] = e.GetComponent<Physics2D>().velocity.x;
		tmpVec2[1] = e.GetComponent<Physics2D>().velocity.y;
		ImGui::DragFloat2("velocity", tmpVec2, 0.1f);
		e.GetComponent<Physics2D>().velocity = { tmpVec2[0] ,tmpVec2[1] };
		SaveUndo(e, tempComponent, COMPONENTID::PHYSICS2D);

		tmpVec2[0] = e.GetComponent<Physics2D>().acceleration.x;
		tmpVec2[1] = e.GetComponent<Physics2D>().acceleration.y;
		ImGui::DragFloat2("acceleration", tmpVec2,0.1f, 0.1f);
		e.GetComponent<Physics2D>().acceleration = { tmpVec2[0] ,tmpVec2[1] };
		SaveUndo(e, tempComponent, COMPONENTID::PHYSICS2D);

				
		tmpVec2[0] = e.GetComponent<Physics2D>().accumulatedForce.x;
		tmpVec2[1] = e.GetComponent<Physics2D>().accumulatedForce.y;
		ImGui::DragFloat2("accumulatedForce", tmpVec2, 0.1f);
		e.GetComponent<Physics2D>().accumulatedForce = { tmpVec2[0] ,tmpVec2[1] };
		SaveUndo(e, tempComponent, COMPONENTID::PHYSICS2D);


		ImGui::DragFloat("angularVelocity", &e.GetComponent<Physics2D>().angularVelocity, 0.1f);
		SaveUndo(e, tempComponent, COMPONENTID::PHYSICS2D);

		ImGui::DragFloat("angularTorque", &e.GetComponent<Physics2D>().angularTorque, 0.1f);
		SaveUndo(e, tempComponent, COMPONENTID::PHYSICS2D);

		ImGui::Checkbox("Physics RenderFlag", &e.GetComponent<Physics2D>().renderFlag);
		SaveUndo(e, tempComponent, COMPONENTID::PHYSICS2D);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.7f, 0.f, 0.f, 1.0f });
		if (ImGui::Button("Remove Physics2D"))
		{
			e.RemoveComponent<Physics2D>();
			LOG_INFO("Physics2D component removed");
		}
		ImGui::PopStyleColor();
	}
}
void InspectorPanel::RectColliderEditor()
{
	if (ImGui::CollapsingHeader("RectCollider")) {
		//ImGui::Text("RectCollider");
		tmpVec2[0] = e.GetComponent<RectCollider>().centerOffset.x;
		tmpVec2[1] = e.GetComponent<RectCollider>().centerOffset.y;
		ImGui::DragFloat2("Box position Offset", tmpVec2, 0.1f);
		e.GetComponent<RectCollider>().centerOffset = { tmpVec2[0] ,tmpVec2[1] };
		SaveUndo(e, tempComponent, COMPONENTID::RECTCOLLIDER);

		tmpVec2[0] = e.GetComponent<RectCollider>().scaleOffset.x;
		tmpVec2[1] = e.GetComponent<RectCollider>().scaleOffset.y;
		ImGui::DragFloat2("Box scale Offset", tmpVec2, 0.1f);
		e.GetComponent<RectCollider>().scaleOffset = { tmpVec2[0] ,tmpVec2[1] };
		SaveUndo(e, tempComponent, COMPONENTID::RECTCOLLIDER);

		ImGui::Checkbox("Collision Trigger", &e.GetComponent<RectCollider>().isTrigger);
		SaveUndo(e, tempComponent, COMPONENTID::RECTCOLLIDER);

		ImGui::Checkbox("Rect RenderFlag", &e.GetComponent<RectCollider>().renderFlag);
		SaveUndo(e, tempComponent, COMPONENTID::RECTCOLLIDER);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.7f, 0.f, 0.f, 1.0f });
		if (ImGui::Button("Remove RectCollider"))
		{
			e.RemoveComponent<RectCollider>();
			LOG_INFO("RectCollider component removed");
		}
		ImGui::PopStyleColor();

	}
}
void InspectorPanel::LayerColliderEditor()
{
	if (ImGui::CollapsingHeader("LayerCollider")) {
		//ImGui::Text("LayerCollider");
		tmpVec2[0] = e.GetComponent<LayerCollider>().centerOffset.x;
		tmpVec2[1] = e.GetComponent<LayerCollider>().centerOffset.y;
		ImGui::DragFloat2("Layer position Offset", tmpVec2, 0.1f);
		e.GetComponent<LayerCollider>().centerOffset = { tmpVec2[0] ,tmpVec2[1] };
		SaveUndo(e, tempComponent, COMPONENTID::LAYERCOLLIDER);

		tmpVec2[0] = e.GetComponent<LayerCollider>().scaleOffset.x;
		tmpVec2[1] = e.GetComponent<LayerCollider>().scaleOffset.y;
		ImGui::DragFloat2("Layer scale Offset", tmpVec2, 0.1f);
		e.GetComponent<LayerCollider>().scaleOffset = { tmpVec2[0] ,tmpVec2[1] };
		SaveUndo(e, tempComponent, COMPONENTID::LAYERCOLLIDER);

		ImGui::Checkbox("Layer RenderFlag", &e.GetComponent<LayerCollider>().renderFlag);
		//BREAKPOINT(e.GetComponent<LayerCollider>().renderFlag == true);
		SaveUndo(e, tempComponent, COMPONENTID::LAYERCOLLIDER);

		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.7f, 0.f, 0.f, 1.0f });
		if (ImGui::Button("Remove LayerCollider"))
		{
			e.RemoveComponent<LayerCollider>();
			LOG_INFO("LayerCollider component removed");
		}
		ImGui::PopStyleColor();

	}
}
void InspectorPanel::CircleColliderEditor()
{
	if (ImGui::CollapsingHeader("CircleCollider")) {
		//ImGui::Text("CircleCollider");
		tmpVec2[0] = e.GetComponent<CircleCollider>().centerOffset.x;
		tmpVec2[1] = e.GetComponent<CircleCollider>().centerOffset.y;
		ImGui::DragFloat2("Circle position Offset", tmpVec2, 0.1f);
		e.GetComponent<CircleCollider>().centerOffset = { tmpVec2[0] ,tmpVec2[1] };
		SaveUndo(e, tempComponent, COMPONENTID::CIRCLECOLLIDER);

		float scale = e.GetComponent<CircleCollider>().scaleOffset;
		ImGui::DragFloat("Circle scale Offset", &scale, 0.1f);
		e.GetComponent<CircleCollider>().scaleOffset = { scale };
		SaveUndo(e, tempComponent, COMPONENTID::CIRCLECOLLIDER);

		ImGui::Checkbox("Collision Trigger", &e.GetComponent<CircleCollider>().isTrigger);
		SaveUndo(e, tempComponent, COMPONENTID::CIRCLECOLLIDER);

		ImGui::Checkbox("Circle RenderFlag", &e.GetComponent<CircleCollider>().renderFlag);
		SaveUndo(e, tempComponent, COMPONENTID::CIRCLECOLLIDER);

		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.7f, 0.f, 0.f, 1.0f });
		if (ImGui::Button("Remove CircleCollider"))
		{
			e.RemoveComponent<CircleCollider>();
			LOG_INFO("CircleCollider component removed");
		}
		ImGui::PopStyleColor();
	}
}
void InspectorPanel::Edge2DColliderEditor()
{
	if (ImGui::CollapsingHeader("Edge2DCollider")) {
		//ImGui::Text("Edge2DCollider");
		tmpVec2[0] = e.GetComponent<Edge2DCollider>().p0Offset.x;
		tmpVec2[1] = e.GetComponent<Edge2DCollider>().p0Offset.y;
		ImGui::DragFloat2("p0 Offset", tmpVec2, 0.1f);
		e.GetComponent<Edge2DCollider>().p0Offset = { tmpVec2[0] ,tmpVec2[1] };
		SaveUndo(e, tempComponent, COMPONENTID::EDGE2DCOLLIDER);

		ImGui::DragFloat("rotationOffset", &e.GetComponent<Edge2DCollider>().rotationOffset, 0.1f);
		SaveUndo(e, tempComponent, COMPONENTID::EDGE2DCOLLIDER);

		ImGui::DragFloat("scaleOffset", &e.GetComponent<Edge2DCollider>().scaleOffset, 0.1f);
		SaveUndo(e, tempComponent, COMPONENTID::EDGE2DCOLLIDER);

		ImGui::Checkbox("RenderFlag", &e.GetComponent<Edge2DCollider>().renderFlag);
		SaveUndo(e, tempComponent, COMPONENTID::EDGE2DCOLLIDER);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.7f, 0.f, 0.f, 1.0f });
		if (ImGui::Button("Remove Edge2DCollider"))
		{
			e.RemoveComponent<Edge2DCollider>();
			LOG_INFO("Edge2DCollider component removed");
		}
		ImGui::PopStyleColor();
	}
}
void InspectorPanel::Point2DColliderEditor()
{
	if (ImGui::CollapsingHeader("Point2DCollider")) {
		//ImGui::Text("Point2DCollider");
		tmpVec2[0] = e.GetComponent<Point2DCollider>().centerOffset.x;
		tmpVec2[1] = e.GetComponent<Point2DCollider>().centerOffset.y;
		ImGui::DragFloat2("centerOffset", tmpVec2, 0.1f);
		e.GetComponent<Point2DCollider>().centerOffset = { tmpVec2[0] ,tmpVec2[1] };
		SaveUndo(e, tempComponent, COMPONENTID::POINT2DCOLLIDER);

		ImGui::Checkbox("RenderFlag", &e.GetComponent<Point2DCollider>().renderFlag);
		SaveUndo(e, tempComponent, COMPONENTID::POINT2DCOLLIDER);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.7f, 0.f, 0.f, 1.0f });
		if (ImGui::Button("Remove Point2DCollider"))
		{
			e.RemoveComponent<Point2DCollider>();
			LOG_INFO("Point2DCollider component removed");
		}
		ImGui::PopStyleColor();
	}
}
void InspectorPanel::AudioEditor()
{
	if (ImGui::CollapsingHeader("Audio")) {
		//ImGui::Text("Audio");
		ImGui::Checkbox("toPlay On start up", &e.GetComponent<Audio>().sound.toPlay);
		SaveUndo(e, tempComponent, COMPONENTID::AUDIO);

		ImGui::Checkbox("toPlayOnCollision", &e.GetComponent<Audio>().sound.toPlayOnCollision);
		SaveUndo(e, tempComponent, COMPONENTID::AUDIO);

		ImGui::InputText("Add Sound Path", &e.GetComponent<Audio>().sound.path);
		SaveUndo(e, tempComponent, COMPONENTID::AUDIO);
		static const wchar_t* texpath = (const wchar_t*)"";
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AUDIO"))
			{
				texpath = (const wchar_t*)payload->Data;
				std::string tp = (std::string)((const char*)texpath);
				e.GetComponent<Audio>().sound.path = tp;
				COMPONENT _new;
				_new = e.GetComponent<Audio>();
				undoStack.push_back(std::make_pair(e, _new));
				stackPointer = (int)undoStack.size();
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::DragFloat("Volume", &e.GetComponent<Audio>().sound.volume, 0.005f, 0.0f, 2.f);
		SaveUndo(e, tempComponent, COMPONENTID::AUDIO);

		ImGui::DragFloat("Pitch", &e.GetComponent<Audio>().sound.pitch, 0.005f,0.5f,2.5f);
		SaveUndo(e, tempComponent, COMPONENTID::AUDIO);

		ImGui::Checkbox("isRandPitch", &e.GetComponent<Audio>().sound.isRandPitch);
		SaveUndo(e, tempComponent, COMPONENTID::AUDIO);

		ImGui::Checkbox("isLoop", &e.GetComponent<Audio>().sound.isLoop);
		SaveUndo(e, tempComponent, COMPONENTID::AUDIO);

		ImGui::Checkbox("isPause", &e.GetComponent<Audio>().sound.isPaused);
		SaveUndo(e, tempComponent, COMPONENTID::AUDIO);


		if (e.HasComponent<Transform>())
		{
			ImGui::Checkbox("isSpacial", &e.GetComponent<Audio>().isSpacial);
			SaveUndo(e, tempComponent, COMPONENTID::AUDIO);

			ImGui::DragFloat("spacialDistance", &e.GetComponent<Audio>().spacialDistance, 0.01f, 0.0f, 10.f);
			SaveUndo(e, tempComponent, COMPONENTID::AUDIO);

			ImGui::DragFloat("spacialRatio", &e.GetComponent<Audio>().spacialRatio, 0.005f, 0.0f, 5.f);
			SaveUndo(e, tempComponent, COMPONENTID::AUDIO);
		}
		
		if (ImGui::Button("Play Now"))
		{
			audioManager->PlayAnySound(e.GetComponent<Audio>().sound.path, 17);
		}
		if (ImGui::Button("Play Sound in Game"))
		{
			//e.GetComponent<Audio>().sound.toPlay = true;
			audioManager->PlaySound(*selectedEntity);
		}
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.7f, 0.f, 0.f, 1.0f });
		if (ImGui::Button("Remove Audio"))
		{
			e.RemoveComponent<Audio>();
			LOG_INFO("Audio component removed");
		}
		ImGui::PopStyleColor();
	}
}
void InspectorPanel::TextEditor()
{
	if (ImGui::CollapsingHeader("Text")) {
		//ImGui::Text("Text");
		//ImGuiInput
		ImGui::InputText("Add text", &e.GetComponent<Text>().text);
		SaveUndo(e, tempComponent, COMPONENTID::TEXT);
		ImGui::InputText("Add font", &e.GetComponent<Text>().fontFile);
		if (ImGui::BeginDragDropTarget())
		{
			static const wchar_t* texpath = (const wchar_t*)"";
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FONT"))
			{
				texpath = (const wchar_t*)payload->Data;
				std::string tp = (std::string)((const char*)texpath);
				e.GetComponent<Text>().fontFile = tp;
			}
			ImGui::EndDragDropTarget();
		}
		SaveUndo(e, tempComponent, COMPONENTID::TEXT);

		tmpVec2[0] = e.GetComponent<Text>().offset.x;
		tmpVec2[1] = e.GetComponent<Text>().offset.y;
		ImGui::DragFloat2("Text Pos", tmpVec2);
		e.GetComponent<Text>().offset = Math::Vec2(tmpVec2[0], tmpVec2[1]);
		SaveUndo(e, tempComponent, COMPONENTID::TEXT);

		ImGui::DragFloat("Text Scale", &e.GetComponent<Text>().scale, 0.01f);
		SaveUndo(e, tempComponent, COMPONENTID::TEXT);

		tmpVec4[0] = e.GetComponent<Text>().color.r / 255.f;
		tmpVec4[1] = e.GetComponent<Text>().color.g / 255.f;
		tmpVec4[2] = e.GetComponent<Text>().color.b / 255.f;
		tmpVec4[3] = e.GetComponent<Text>().color.a / 255.f;
		
		ImGui::ColorEdit4("Text Color", tmpVec4);
		e.GetComponent<Text>().color.r = (GLubyte)(tmpVec4[0] * 255);
		e.GetComponent<Text>().color.g = (GLubyte)(tmpVec4[1] * 255);
		e.GetComponent<Text>().color.b = (GLubyte)(tmpVec4[2] * 255);
		e.GetComponent<Text>().color.a = (GLubyte)(tmpVec4[3] * 255);
		ImGui::Checkbox("Follow Cam", &e.GetComponent<Text>().followCam);
		ImGui::DragFloat("Text Width", &e.GetComponent<Text>().width);

		SaveUndo(e, tempComponent, COMPONENTID::TEXT);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.7f, 0.f, 0.f, 1.0f });
		if (ImGui::Button("Remove Text"))
		{
			e.RemoveComponent<Text>();
			LOG_INFO("Text component removed");
		}
		ImGui::PopStyleColor();
	}
}
void InspectorPanel::AIEditor()
{

	if (ImGui::CollapsingHeader("AI")) {
		//static const char* colorChange[]{ "None","Smoothy","Traffic Light" };
		int colorChangeID = e.GetComponent<AI>().colorChange;
		ImGui::Combo("Select Color Change", &colorChangeID, colorChange, IM_ARRAYSIZE(colorChange));
		e.GetComponent<AI>().colorChange = colorChangeID;
		SaveUndo(e, tempComponent, COMPONENTID::AI);

		//static const char* movement[]{ "None","UP-Down","Left-Right", "Swing", "Circle" };
		int movementID = e.GetComponent<AI>().movement;
		ImGui::Combo("Select Movement", &movementID, movement, IM_ARRAYSIZE(movement));
		e.GetComponent<AI>().movement = movementID;
		SaveUndo(e, tempComponent, COMPONENTID::AI);

		if (e.GetComponent<AI>().movement)
		{
			float speed = e.GetComponent<AI>().speed;
			ImGui::SliderFloat("Speed", &speed, 0.f, 15.f);
			e.GetComponent<AI>().speed = speed;
			SaveUndo(e, tempComponent, COMPONENTID::AI);
			if (e.GetComponent<AI>().movement < 4)
			{
				float range = e.GetComponent<AI>().range;
				ImGui::SliderFloat("Range", &range, 0.f, 400.f);
				e.GetComponent<AI>().range = range;
				SaveUndo(e, tempComponent, COMPONENTID::AI);
			}
		}
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.7f, 0.f, 0.f, 1.0f });
		if (ImGui::Button("Remove AI"))
		{
			e.RemoveComponent<AI>();
			LOG_INFO("AI component removed");
		}
		ImGui::PopStyleColor();
	}
}
void InspectorPanel::ScriptEditor()
{
	if (ImGui::CollapsingHeader("Script")) {
		if (ImGui::InputText("Add script file name", &e.GetComponent<Script>().name))
		{
			//e.GetComponent<Script>().script = ScriptManager<ScriptComponent>::GetInstance()->GetScript(e.GetComponent<Script>().name);
		}
		if (ImGui::BeginDragDropTarget())
		{
			static const wchar_t* texpath = (const wchar_t*)"";
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCRIPT"))
			{
				texpath = (const wchar_t*)payload->Data;
				std::string tp = (std::string)((const char*)texpath);
				e.GetComponent<Script>().name = tp;
				//e.GetComponent<Script>().script = ScriptManager<ScriptComponent>::GetInstance()->GetScript(e.GetComponent<Script>().name);
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.7f, 0.f, 0.f, 1.0f });
		if (ImGui::Button("Remove Script"))
		{
			e.RemoveComponent<Script>();
			LOG_INFO("Script component removed");
		}
		ImGui::PopStyleColor();
	}
}
void InspectorPanel::DialogueEditor()
{
	if (ImGui::CollapsingHeader("Dialogue")) 
	{
		ImGui::InputText("Add Dialogue file name", &e.GetComponent<Dialogue>().filename);
		if (ImGui::BeginDragDropTarget())
		{
			static const wchar_t* texpath = (const wchar_t*)"";
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DIALOGUE"))
			{
				texpath = (const wchar_t*)payload->Data;
				std::string tp = (std::string)((const char*)texpath);
				e.GetComponent<Dialogue>().filename = tp;
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::InputInt("speakerID", &e.GetComponent<Dialogue>().speakerID);
		e.GetComponent<Dialogue>().speakerID = e.GetComponent<Dialogue>().speakerID < 0 ? 0 
			: (e.GetComponent<Dialogue>().speakerID > 1 ? 1
				: e.GetComponent<Dialogue>().speakerID);
		SaveUndo(e, tempComponent, COMPONENTID::DIALOGUE);
		ImGui::InputInt("selectedID", &e.GetComponent<Dialogue>().selectedID);

		ImGui::InputInt("textID", &e.GetComponent<Dialogue>().textID);

		ImGui::InputInt("nextTextID", &e.GetComponent<Dialogue>().nextTextID);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.7f, 0.f, 0.f, 1.0f });
		if (ImGui::Button("Remove Dialogue"))
		{
			e.RemoveComponent<Dialogue>();
			LOG_INFO("Dialogue component removed");
		}
		ImGui::PopStyleColor();
	}
}
void InspectorPanel::ButtonEditor()
{
	if (ImGui::CollapsingHeader("Button"))
	{
		ImGui::Checkbox("interactable", &e.GetComponent<Button>().interactable);
		SaveUndo(e, tempComponent, COMPONENTID::BUTTON);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.7f, 0.f, 0.f, 1.0f });
		if (ImGui::Button("Remove Button"))
		{
			e.RemoveComponent<Button>();
			LOG_INFO("Button component removed");
		}
		ImGui::PopStyleColor();
	}

}
void InspectorPanel::LightSourceEditor()
{
	if (ImGui::CollapsingHeader("LightSource"))
	{
		tmpVec2[0] = e.GetComponent<LightSource>().centerOffset.x;
		tmpVec2[1] = e.GetComponent<LightSource>().centerOffset.y;
		ImGui::DragFloat2("Light center", tmpVec2);
		e.GetComponent<LightSource>().centerOffset = { tmpVec2[0] ,tmpVec2[1] };

		ImGui::DragFloat("Radius", &e.GetComponent<LightSource>().radius, 1.f);

		ImGui::DragFloat("Intensity", &e.GetComponent<LightSource>().intensity, 0.05f, 0.f, 1.f);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.7f, 0.f, 0.f, 1.0f });
		if (ImGui::Button("Remove LightSource"))
		{
			e.RemoveComponent<LightSource>();
			LOG_INFO("LightSource component removed");
		}
		ImGui::PopStyleColor();
	}
}
void InspectorPanel::ShadowCasterEditor()
{
	if (ImGui::CollapsingHeader("ShadowCaster"))
	{
		static bool pp = false;
		if (pp)
		{
			e.GetComponent<ShadowCaster>().renderFlag = true;
			SRT = 5;
			Math::Vec2 point = mWorldMousePos * renderManager->GetWorldCamera().GetZoom() + renderManager->GetWorldCamera().GetPos() - e.GetComponent<Transform>().translation;
			point.x = (float)((int)point.x);
			point.y = (float)((int)point.y);
			if (Input::CheckKey(E_STATE::PRESS, E_KEY::M_BUTTON_L))
			{
				e.GetComponent<ShadowCaster>().centerOffset.push_back(Math::Vec2{ point });
			}
			else if (Input::CheckKey(E_STATE::HOLD, E_KEY::M_BUTTON_L))
			{
				e.GetComponent<ShadowCaster>().centerOffset[e.GetComponent<ShadowCaster>().centerOffset.size() - 1] = point;
			}
			else if (Input::CheckKey(E_STATE::HOLD, E_KEY::M_BUTTON_R))
			{
				e.GetComponent<ShadowCaster>().centerOffset[e.GetComponent<ShadowCaster>().centerOffset.size() - 1] = point;
			}
		}
		
		ImGui::Checkbox("Point Picker", &pp);
		if(!pp)
		{
			if (SRT == 5)
			{
				e.GetComponent<ShadowCaster>().centerOffset.pop_back();
				SRT = 0;
			}
		}
		for (int i = 0; i < e.GetComponent<ShadowCaster>().centerOffset.size(); i++)
		{
			tmpVec2[0] = e.GetComponent<ShadowCaster>().centerOffset[i].x;
			tmpVec2[1] = e.GetComponent<ShadowCaster>().centerOffset[i].y;
			ImGui::DragFloat2(("Shadow center" + std::to_string(i)).c_str(), tmpVec2);
			e.GetComponent<ShadowCaster>().centerOffset[i] = {tmpVec2[0] ,tmpVec2[1]};

			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.7f, 0.f, 0.f, 1.0f });
			if (ImGui::Button(("Remove Shadow Vertex" + std::to_string(i)).c_str()))
			{
				e.GetComponent<ShadowCaster>().centerOffset.erase(e.GetComponent<ShadowCaster>().centerOffset.begin()+i);
			}
			ImGui::PopStyleColor();
			ImGui::Separator();
		}
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.0f, 0.5f, 0.f, 1.0f });
		if (ImGui::Button("Add Shadow Vertex"))
		{
			e.GetComponent<ShadowCaster>().centerOffset.push_back(Math::Vec2{});
		}
		ImGui::PopStyleColor();
		ImGui::Checkbox("Shadow RenderFlag", &e.GetComponent<ShadowCaster>().renderFlag);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.7f, 0.f, 0.f, 1.0f });
		if (ImGui::Button("Remove ShadowCaster"))
		{
			e.RemoveComponent<ShadowCaster>();
			LOG_INFO("ShadowCaster component removed");
		}
		ImGui::PopStyleColor();
	}
}
void InspectorPanel::ViewportEditor()
{
	if (ImGui::CollapsingHeader("Viewport"))
	{
		ImGui::Text("Viewport");
		//int typeID = (int)e.GetComponent<Viewport>().viewport;
		//static const char* type[]{ "RECTANGULAR", "CIRCULAR"};
		//ImGui::Combo("ViewportType", &typeID, type, IM_ARRAYSIZE(type));
		//e.GetComponent<Viewport>().viewport = (VIEWPORT)typeID;
		ImGui::Checkbox("isUI", &e.GetComponent<Viewport>().isUI);
		ImGui::DragInt("Viewport width", &e.GetComponent<Viewport>().width, 1, 0, 1600);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.7f, 0.f, 0.f, 1.0f });
		if (ImGui::Button("Remove Viewport"))
		{
			e.RemoveComponent<Viewport>();
			LOG_INFO("Viewport component removed");
		}
		ImGui::PopStyleColor();
	}
}
void InspectorPanel::MovementAIEditor()
{
	if (ImGui::CollapsingHeader("MovementAI"))
	{
		ImGui::Text((std::to_string(e.GetComponent<MovementAI>().step)).c_str());
		ImGui::Checkbox("run_m", &e.GetComponent<MovementAI>().run);
		ImGui::Checkbox("next_m", &e.GetComponent<MovementAI>().next);
		ImGui::Checkbox("loop_m", &e.GetComponent<MovementAI>().loop);
		ImGui::Checkbox("reverse_m", &e.GetComponent<MovementAI>().reverse);
		ImGui::Checkbox("cycle_m", &e.GetComponent<MovementAI>().cycle);
		ImGui::Separator();
		ImGui::Checkbox("moveOnHover", &e.GetComponent<MovementAI>().moveOnHover);
		ImGui::Checkbox("moveOnCollide", &e.GetComponent<MovementAI>().moveOnCollide);
		ImGui::Separator();
		ImGui::Checkbox("moveScale", &e.GetComponent<MovementAI>().moveScale);
		ImGui::Checkbox("moveRotate", &e.GetComponent<MovementAI>().moveRotate);
		ImGui::Checkbox("moveTranslation", &e.GetComponent<MovementAI>().moveTranslation);

		ImGui::Separator();
		static bool pp = false;
		if (pp)
		{
			e.GetComponent<MovementAI>().renderFlag = true;
			SRT = 6;
			Math::Vec2 point = mWorldMousePos * renderManager->GetWorldCamera().GetZoom() + renderManager->GetWorldCamera().GetPos();// -e.GetComponent<Transform>().translation;
			if (Input::CheckKey(E_STATE::PRESS, E_KEY::M_BUTTON_L))
			{
				e.GetComponent<MovementAI>().time.push_back(e.GetComponent<MovementAI>().time[e.GetComponent<MovementAI>().targetTransforms.size() - 1]);
				e.GetComponent<MovementAI>().targetTransforms.push_back(e.GetComponent<MovementAI>().targetTransforms[e.GetComponent<MovementAI>().targetTransforms.size() - 1]);
			}
			else if (Input::CheckKey(E_STATE::HOLD, E_KEY::M_BUTTON_L))
			{
				e.GetComponent<MovementAI>().targetTransforms[e.GetComponent<MovementAI>().targetTransforms.size() - 1].translation = point;
			}
			else if (Input::CheckKey(E_STATE::HOLD, E_KEY::M_BUTTON_R))
			{
				e.GetComponent<MovementAI>().targetTransforms[e.GetComponent<MovementAI>().targetTransforms.size() - 1].translation = point;
			}
		}

		ImGui::Checkbox("Translation Point Adder", &pp);
		if (!pp)
		{
			if (SRT == 6)
			{
				e.GetComponent<MovementAI>().targetTransforms.pop_back();
				SRT = 0;
			}
		}
		for (int i = 0; i < e.GetComponent<MovementAI>().targetTransforms.size(); i++)
		{
			ImGui::DragFloat(("Set mTime" + std::to_string(i)).c_str(), &e.GetComponent<MovementAI>().time[i], 0.1f, 0.f, 60.f);

			//scale
			tmpVec2[0] = e.GetComponent<MovementAI>().targetTransforms[i].scale.x;
			tmpVec2[1] = e.GetComponent<MovementAI>().targetTransforms[i].scale.y;
			ImGui::DragFloat2(("Set Target Scale" + std::to_string(i)).c_str(), tmpVec2);
			e.GetComponent<MovementAI>().targetTransforms[i].scale = Math::Vec2(tmpVec2[0], tmpVec2[1]);

			//translate
			tmpVec2[0] = e.GetComponent<MovementAI>().targetTransforms[i].translation.x;
			tmpVec2[1] = e.GetComponent<MovementAI>().targetTransforms[i].translation.y;
			ImGui::DragFloat2(("Set Target Position" + std::to_string(i)).c_str(), tmpVec2);
			e.GetComponent<MovementAI>().targetTransforms[i].translation = Math::Vec2(tmpVec2[0], tmpVec2[1]);

			//rotate
			tmpFloat = e.GetComponent<MovementAI>().targetTransforms[i].rotation;
			tmpFloat = (float)(tmpFloat / M_PI * 180.f);
			ImGui::DragFloat(("Set Target Rotation" + std::to_string(i)).c_str(), &tmpFloat, 1.f, -360.f, 360.f);
			tmpFloat = (float)(tmpFloat * M_PI / 180.f);
			e.GetComponent<MovementAI>().targetTransforms[i].rotation = tmpFloat;
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.7f, 0.f, 0.f, 1.0f });
			if (ImGui::Button(("Remove Transform" + std::to_string(i)).c_str()))
			{
				movementAIManager->RemoveTransformAt(e, i);
				//e.GetComponent<MovementAI>().time.erase(e.GetComponent<MovementAI>().time.begin() + i);
				//e.GetComponent<MovementAI>().targetTransforms.erase(e.GetComponent<MovementAI>().targetTransforms.begin() + i);
			}
			ImGui::PopStyleColor();
			ImGui::Separator();
		}
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.0f, 0.5f, 0.f, 1.0f });
		if (ImGui::Button("Add Transform"))
		{
			//e.GetComponent<MovementAI>().targetTransforms.push_back(Transform{});
			//e.GetComponent<MovementAI>().time.push_back(1.f);
			movementAIManager->AddTransform(e, e.GetComponent<Transform>());
		}
		ImGui::Checkbox("AI render Flag", &e.GetComponent<MovementAI>().renderFlag);
		ImGui::PopStyleColor();
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.7f, 0.f, 0.f, 1.0f });
		if (ImGui::Button("Remove MovementAI"))
		{
			e.RemoveComponent<MovementAI>();
			LOG_INFO("MovementAI component removed");
		}
		ImGui::PopStyleColor();


	}

}
void InspectorPanel::ColorAIEditor()
{
	if (ImGui::CollapsingHeader("ColorAI"))
	{
		ImGui::Text((std::to_string(e.GetComponent<ColorAI>().step)).c_str());
		ImGui::Checkbox("run_c", &e.GetComponent<ColorAI>().run);
		ImGui::Checkbox("next_c", &e.GetComponent<ColorAI>().next);
		ImGui::Checkbox("loop_c", &e.GetComponent<ColorAI>().loop);
		ImGui::Checkbox("reverse_c", &e.GetComponent<ColorAI>().reverse);
		ImGui::Checkbox("cycle_c", &e.GetComponent<ColorAI>().cycle);
		ImGui::Separator();
		ImGui::Checkbox("changeOnHover", &e.GetComponent<ColorAI>().changeOnHover);
		ImGui::Checkbox("changeOnCollide", &e.GetComponent<ColorAI>().changeOnCollide);
		ImGui::Separator();
		ImGui::Checkbox("changeR", &e.GetComponent<ColorAI>().changeR);
		ImGui::Checkbox("changeG", &e.GetComponent<ColorAI>().changeG);
		ImGui::Checkbox("changeB", &e.GetComponent<ColorAI>().changeB);
		ImGui::Checkbox("changeA", &e.GetComponent<ColorAI>().changeA);
		int spriteORtextID = e.GetComponent<ColorAI>().spriteORtext;
		ImGui::Combo("Select sprite OR text", &spriteORtextID, spriteORtext, IM_ARRAYSIZE(spriteORtext));
		if((spriteORtextID == 0)||(e.HasComponent<Text>()))
			e.GetComponent<ColorAI>().spriteORtext = spriteORtextID;
		ImGui::Separator();
		
		for (int i = 0; i < e.GetComponent<ColorAI>().targetColors.size(); i++)
		{
			ImGui::DragFloat(("Set cTime" + std::to_string(i)).c_str(), &e.GetComponent<ColorAI>().time[i], 0.1f, 0.f, 60.f);

			//color
			tmpVec4[0] = e.GetComponent<ColorAI>().targetColors[i].r / 255.f;
			tmpVec4[1] = e.GetComponent<ColorAI>().targetColors[i].g / 255.f;
			tmpVec4[2] = e.GetComponent<ColorAI>().targetColors[i].b / 255.f;
			tmpVec4[3] = e.GetComponent<ColorAI>().targetColors[i].a / 255.f;
			ImGui::ColorEdit4(("AI Color" + std::to_string(i)).c_str(), tmpVec4);
			e.GetComponent<ColorAI>().targetColors[i].r = (GLubyte)(tmpVec4[0] * 255);
			e.GetComponent<ColorAI>().targetColors[i].g = (GLubyte)(tmpVec4[1] * 255);
			e.GetComponent<ColorAI>().targetColors[i].b = (GLubyte)(tmpVec4[2] * 255);
			e.GetComponent<ColorAI>().targetColors[i].a = (GLubyte)(tmpVec4[3] * 255);

			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.7f, 0.f, 0.f, 1.0f });
			if (ImGui::Button(("Remove Color" + std::to_string(i)).c_str()))
			{
				colorAIManager->RemoveColorAt(e, i);
				//e.GetComponent<ColorAI>().time.erase(e.GetComponent<ColorAI>().time.begin() + i);
				//e.GetComponent<ColorAI>().targetColors.erase(e.GetComponent<ColorAI>().targetColors.begin() + i);
			}
			ImGui::PopStyleColor();
			ImGui::Separator();
		}
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.0f, 0.5f, 0.f, 1.0f });
		if (ImGui::Button("Add Color"))
		{
			if(e.HasComponent<Text>()&&e.GetComponent<ColorAI>().spriteORtext ==1)
				colorAIManager->AddColor(e, e.GetComponent<Text>().color);
			else //sprite or both
				colorAIManager->AddColor(e, e.GetComponent<Sprite>().color);
		}
		ImGui::PopStyleColor();
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.7f, 0.f, 0.f, 1.0f });
		if (ImGui::Button("Remove ColorAI"))
		{
			e.RemoveComponent<ColorAI>();
			LOG_INFO("ColorAI component removed");
		}
		ImGui::PopStyleColor();


	}

}
void InspectorPanel::ParticleSystemEditor()
{
	if (ImGui::CollapsingHeader("ParticleSystem"))
	{
		ImGui::InputFloat("mScale", &e.GetComponent<ParticleSystem>().mParticleInfo.mScale);
		ImGui::InputFloat("mFacing", &e.GetComponent<ParticleSystem>().mParticleInfo.mFacing);
		ImGui::InputFloat("mLifespan", &e.GetComponent<ParticleSystem>().mParticleInfo.mLifespan);

		ImGui::Text("Sprite");
		tmpVec4[0] = e.GetComponent<ParticleSystem>().mParticleInfo.mSprite.color.r / 255.f;
		tmpVec4[1] = e.GetComponent<ParticleSystem>().mParticleInfo.mSprite.color.g / 255.f;
		tmpVec4[2] = e.GetComponent<ParticleSystem>().mParticleInfo.mSprite.color.b / 255.f;
		tmpVec4[3] = e.GetComponent<ParticleSystem>().mParticleInfo.mSprite.color.a / 255.f;
		ImGui::ColorEdit4("mSprite Color", tmpVec4);
		e.GetComponent<ParticleSystem>().mParticleInfo.mSprite.color.r = (GLubyte)(tmpVec4[0] * 255);
		e.GetComponent<ParticleSystem>().mParticleInfo.mSprite.color.g = (GLubyte)(tmpVec4[1] * 255);
		e.GetComponent<ParticleSystem>().mParticleInfo.mSprite.color.b = (GLubyte)(tmpVec4[2] * 255);
		e.GetComponent<ParticleSystem>().mParticleInfo.mSprite.color.a = (GLubyte)(tmpVec4[3] * 255);

		std::string tex{};
		if (e.GetComponent<ParticleSystem>().mParticleInfo.mSprite.sprite == SPRITE::TEXTURE)
		{
			tex = spriteManager->GetTexturePath(e.GetComponent<ParticleSystem>().mParticleInfo.mSprite.texture);
		}
		else if (e.GetComponent<ParticleSystem>().mParticleInfo.mSprite.sprite == SPRITE::CIRCLE)
			tex = "CIRCLE";
		else if (e.GetComponent<ParticleSystem>().mParticleInfo.mSprite.sprite == SPRITE::SQUARE)
			tex = "SQUARE";
		int shapeID = (int)e.GetComponent<ParticleSystem>().mParticleInfo.mSprite.sprite;
		//static const char* shape[]{ "SQUARE", "CIRCLE", "TEXTURE","DEBUG_POINT" , "DEBUG_LINE","DEBUG_SQUARE","DEBUG_CIRCLE", "DEBUG_ARROW" };
		ImGui::Combo("mShape", &shapeID, shape, IM_ARRAYSIZE(shape));
		e.GetComponent<ParticleSystem>().mParticleInfo.mSprite.sprite = (SPRITE)shapeID;

		if ((SPRITE)shapeID != SPRITE::TEXTURE)
		{
			e.GetComponent<ParticleSystem>().mParticleInfo.mSprite.texture = 0;
		}
		if (e.GetComponent<ParticleSystem>().mParticleInfo.mSprite.sprite == SPRITE::TEXTURE)
		{
			
			ImGui::InputText("mSprite", &tex);
			e.GetComponent<ParticleSystem>().mParticleInfo.mSprite.texture = spriteManager->GetTextureID(tex);
		}
		else
		{
			ImGui::Text(tex.c_str());
		}
		ImGui::InputInt("mlayer", &e.GetComponent<ParticleSystem>().mParticleInfo.mSprite.layer);

		ImGui::InputFloat("mRotation", &e.GetComponent<ParticleSystem>().mParticleInfo.mRotation);
		ImGui::InputFloat("mSpeed", &e.GetComponent<ParticleSystem>().mParticleInfo.mSpeed);
		ImGui::Checkbox("mFading", &e.GetComponent<ParticleSystem>().mParticleInfo.mFading);
		ImGui::Checkbox("mFadeIn", &e.GetComponent<ParticleSystem>().mParticleInfo.mFadeIn);



		ImGui::InputInt("mDensity", &e.GetComponent<ParticleSystem>().mDensity);
		tmpVec2[0] = e.GetComponent<ParticleSystem>().mCenter.x;
		tmpVec2[1] = e.GetComponent<ParticleSystem>().mCenter.y;
		ImGui::DragFloat2("mCenter", tmpVec2);
		e.GetComponent<ParticleSystem>().mCenter = { tmpVec2[0] ,tmpVec2[1] };
		ImGui::InputFloat("mAreaWidth", &e.GetComponent<ParticleSystem>().mAreaWidth);
		ImGui::InputFloat("mDirection", &e.GetComponent<ParticleSystem>().mDirection);
		ImGui::InputFloat("mSpread", &e.GetComponent<ParticleSystem>().mSpread);
		ImGui::InputFloat("mDuration", &e.GetComponent<ParticleSystem>().mDuration);
		ImGui::Checkbox("mIsActive", &e.GetComponent<ParticleSystem>().mIsActive);
		ImGui::InputFloat("mSlow", &e.GetComponent<ParticleSystem>().mSlow);
	}

}
/*!*****************************************************************************
\brief
	This functions below onawards edits a particular component in a prefab

\return
None.
*******************************************************************************/
void InspectorPanel::PrefabEditor()
{
	if (p == nullptr)
		return;
	if (p->HasComponent<General>())
	{
		ImGui::InputText("Prefab Name", &p->Name());
		if (ImGui::CollapsingHeader("General") || true) {
			ImGui::Text("General");
			//General general = p->GetComponent<General>();
			//ImGui::Checkbox("isActive", &general.isActive); //isactive
			//
			////Should not chnage general name for all prefabees
			//ImGui::InputText("Name", &general.name);


			//int tagID = (int)general.tag;
			////static const char* tag[]{ "PLAYER","PASSENGER", "ENEMY", "BUILDING","BACKGROUND", "ENVIRONMENT","EFFECTS","PREFABS","OTHERS"};
			//ImGui::Combo("Tag", &tagID, tag, IM_ARRAYSIZE(tag));
			//general.tag = (TAG)tagID;


			//int subtagID = (int)general.subtag;
			////static const char* subtag[]{ "NOSUBTAG", "PLAYER", "PASSENGER", "ENEMY", "BUILDING", "OTHERS" };
			//ImGui::Combo("SubTag", &subtagID, subtag, IM_ARRAYSIZE(subtag));
			//general.subtag = (SUBTAG)subtagID;
			//p->UpdateComponent(general);
		}
	}
	if (p->HasComponent<Lifespan>())
	{
		if (ImGui::CollapsingHeader("Lifespan")) {
			//ImGui::Text("Lifespan");
			Lifespan lifespan = p->GetComponent<Lifespan>();
			ImGui::InputFloat("Lifespan", &lifespan.limit);
			p->UpdateComponent(lifespan);
			if (ImGui::Button("Remove Lifespan"))
			{
				p->RemoveComponent<Lifespan>();
				LOG_INFO("Lifespan component removed");
			}
		}
	}

	if (p->HasComponent<Transform>())
	{
		if (ImGui::CollapsingHeader("Transform")) {
			Transform transform = p->GetComponent<Transform>();
			//ImGui::Text("Transform Component");
			tmpVec2[0] = transform.scale.x;
			tmpVec2[1] = transform.scale.y;
			ImGui::DragFloat2("Set Scale", tmpVec2);
			transform.scale = { tmpVec2[0] ,tmpVec2[1] };

			tmpVec2[0] = transform.translation.x;
			tmpVec2[1] = transform.translation.y;
			ImGui::DragFloat2("Set Position", tmpVec2);
			transform.translation = { tmpVec2[0] ,tmpVec2[1] };

			tmpFloat = transform.rotation;
			tmpFloat = (float)(tmpFloat / M_PI * 180.f);
			ImGui::DragFloat("Set Rotation", &tmpFloat, -360.f, 360.f);
			tmpFloat = (float)(tmpFloat * M_PI / 180.f);
			transform.rotation = tmpFloat;
			p->UpdateComponent(transform);
			if (ImGui::Button("Remove Transform"))
			{
				p->RemoveComponent<Transform>();
				LOG_INFO("Transform component removed");
			}
		}

	}

	if (p->HasComponent<Sprite>())
	{
		if (ImGui::CollapsingHeader("Sprite")) {
			//ImGui::Text("Sprite");
			Sprite sprite = p->GetComponent<Sprite>();
			tmpVec4[0] = sprite.color.r / 255.f;
			tmpVec4[1] = sprite.color.g / 255.f;
			tmpVec4[2] = sprite.color.b / 255.f;
			tmpVec4[3] = sprite.color.a / 255.f;
			ImGui::ColorEdit4("Color", tmpVec4);
			sprite.color.r = (GLubyte)(tmpVec4[0] * 255);
			sprite.color.g = (GLubyte)(tmpVec4[1] * 255);
			sprite.color.b = (GLubyte)(tmpVec4[2] * 255);
			sprite.color.a = (GLubyte)(tmpVec4[3] * 255);
			std::string tex{};
			if (sprite.sprite == SPRITE::TEXTURE)
				tex = spriteManager->GetTexturePath(sprite.texture);
			else if (sprite.sprite == SPRITE::CIRCLE)
				tex = "CIRCLE";
			else if (sprite.sprite == SPRITE::SQUARE)
				tex = "SQUARE";

			int shapeID = (int)sprite.sprite;
			//static const char* shape[]{ "SQUARE", "CIRCLE", "TEXTURE","DEBUG_POINT" , "DEBUG_LINE","DEBUG_SQUARE","DEBUG_CIRCLE", "DEBUG_ARROW" };
			ImGui::Combo("Shape", &shapeID, shape, IM_ARRAYSIZE(shape));
			sprite.sprite = (SPRITE)shapeID;
			if ((SPRITE)shapeID != SPRITE::TEXTURE)
			{
				sprite.texture = 0;
			}
			ImGui::Text(tex.c_str());

			static const wchar_t* texpath = (const wchar_t*)"";
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURES"))
				{
					texpath = (const wchar_t*)payload->Data;
					std::string tp = (std::string)((const char*)texpath);
					sprite.sprite = SPRITE::TEXTURE;
					sprite.texture = spriteManager->GetTextureID(tp);
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::InputInt("Layer", &sprite.layer);
			p->UpdateComponent(sprite);
			if (ImGui::Button("Remove Sprite"))
			{
				p->RemoveComponent<Sprite>();
				LOG_INFO("Sprite component removed");
			}
		}
	}
	if (p->HasComponent<Animation>())
	{
		if (ImGui::CollapsingHeader("Animation")) {
			//ImGui::Text("Animation");
			//Animation animation = p->GetComponent<Animation>();
			//static GLuint addImage = {};
			//static std::string  texadd = "Add image";
			//for (size_t i = 0; i <= animation.images.size(); ++i)
			//{
			//	std::string tex{};
			//	if (i != animation.images.size())
			//	{
			//		tex = spriteManager->GetTexturePath(animation.images[i]);
			//		ImGui::Text(tex.c_str());
			//	}
			//	else
			//	{
			//		ImGui::InputText("Addimage", &texadd);
			//		tex = texadd;
			//	}
			//	static const wchar_t* texpath = (const wchar_t*)"";
			//	if (ImGui::BeginDragDropTarget())
			//	{
			//		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURES"))
			//		{
			//			texpath = (const wchar_t*)payload->Data;
			//			if (i == p->GetComponent<Animation>().images.size())
			//				texadd = (char*)texpath;
			//			std::string  tp = (std::string)((const char*)texpath);
			//			if (i != animation.images.size())
			//				animation.images[i] = spriteManager->GetTextureID(tp);
			//			else
			//				addImage = spriteManager->GetTextureID(tp);
			//		}
			//		ImGui::EndDragDropTarget();
			//	}
			//}
			//if (ImGui::Button("Add Sprite"))
			//{
			//	animator->AddImages(e, addImage);
			//}
			//ImGui::InputFloat("timePerImage", &animation.timePerImage[e.GetComponent<Animation>().currentImageIndex]); //yj
			//ImGui::InputFloat("timeToImageSwap", &animation.timeToImageSwap);
			//ImGui::InputInt("currentImageIndex", &animation.currentImageIndex);
			//p->UpdateComponent<Animation>(animation);
			if (ImGui::Button("Remove Animation"))
			{
				p->RemoveComponent<Animation>();
				LOG_INFO("Animation component removed");
			}
		}
	}
	if (p->HasComponent<SheetAnimation>())
	{
		if (ImGui::CollapsingHeader("SheetAnimation")) {
			//ImGui::Text("SheetAnimation");
			SheetAnimation sheetAnimation = p->GetComponent<SheetAnimation>();
			ImGui::InputInt("frameCount", &sheetAnimation.frameCount);
			ImGui::InputInt("currFrameIndex", &sheetAnimation.currFrameIndex);
			ImGui::InputFloat("timePerFrame", &sheetAnimation.timePerFrame);
			ImGui::InputFloat("timeToFrameSwap", &sheetAnimation.timeToFrameSwap);
			p->UpdateComponent<SheetAnimation>(sheetAnimation);
			if (ImGui::Button("Remove SheetAnimation"))
			{
				p->RemoveComponent<SheetAnimation>();
				LOG_INFO("SheetAnimation component removed");
			}
		}
	}
	if (p->HasComponent<RectCollider>())
	{
		if (ImGui::CollapsingHeader("RectCollider")) {
			//ImGui::Text("RectCollider");
			RectCollider rectCollider = p->GetComponent<RectCollider>();
			tmpVec2[0] = p->GetComponent<RectCollider>().centerOffset.x;
			tmpVec2[1] = p->GetComponent<RectCollider>().centerOffset.y;
			ImGui::InputFloat2("Box position Offset", tmpVec2);
			rectCollider.centerOffset = { tmpVec2[0] ,tmpVec2[1] };

			tmpVec2[0] = rectCollider.scaleOffset.x;
			tmpVec2[1] = rectCollider.scaleOffset.y;
			ImGui::InputFloat2("Box scale Offset", tmpVec2);
			rectCollider.scaleOffset = { tmpVec2[0] ,tmpVec2[1] };

			ImGui::Checkbox("Rect RenderFlag", &rectCollider.renderFlag);
			p->UpdateComponent<RectCollider>(rectCollider);
			if (ImGui::Button("Remove RectCollider"))
			{
				p->RemoveComponent<RectCollider>();
				LOG_INFO("RectCollider component removed");
			}
		}
	}
	if (p->HasComponent<CircleCollider>())
	{
		if (ImGui::CollapsingHeader("CircleCollider")) {
			//ImGui::Text("CircleCollider");
			CircleCollider circleCollider = p->GetComponent<CircleCollider>();
			tmpVec2[0] = circleCollider.centerOffset.x;
			tmpVec2[1] = circleCollider.centerOffset.y;
			ImGui::InputFloat2("Circle position Offset", tmpVec2);
			circleCollider.centerOffset = { tmpVec2[0] ,tmpVec2[1] };

			float scale = circleCollider.scaleOffset;
			ImGui::InputFloat("Circle scale Offset", &scale);
			circleCollider.scaleOffset = { scale };

			ImGui::Checkbox("Circle RenderFlag", &circleCollider.renderFlag);

			p->UpdateComponent<CircleCollider>(circleCollider);
			if (ImGui::Button("Remove CircleCollider"))
			{
				p->RemoveComponent<CircleCollider>();
				LOG_INFO("CircleCollider component removed");
			}
		}
	}
	if (p->HasComponent<Edge2DCollider>())
	{
		if (ImGui::CollapsingHeader("Edge2DCollider")) {
			//ImGui::Text("Edge2DCollider");
			Edge2DCollider edge2DCollider = p->GetComponent<Edge2DCollider>();
			tmpVec2[0] = edge2DCollider.p0Offset.x;
			tmpVec2[1] = edge2DCollider.p0Offset.y;
			ImGui::InputFloat2("p0 Offset", tmpVec2);
			edge2DCollider.p0Offset = { tmpVec2[0] ,tmpVec2[1] };

			ImGui::InputFloat("rotationOffset", &edge2DCollider.rotationOffset);
			ImGui::InputFloat("scaleOffset", &edge2DCollider.scaleOffset);
			ImGui::Checkbox("RenderFlag", &edge2DCollider.renderFlag);
			p->UpdateComponent<Edge2DCollider>(edge2DCollider);
			if (ImGui::Button("Remove Edge2DCollider"))
			{
				p->RemoveComponent<Edge2DCollider>();
				LOG_INFO("Edge2DCollider component removed");
			}
		}
	}
	if (p->HasComponent<Point2DCollider>())
	{
		if (ImGui::CollapsingHeader("Point2DCollider")) {
			//ImGui::Text("Point2DCollider");
			Point2DCollider point2DCollider = p->GetComponent<Point2DCollider>();
			tmpVec2[0] = point2DCollider.centerOffset.x;
			tmpVec2[1] = point2DCollider.centerOffset.y;
			ImGui::InputFloat2("centerOffset", tmpVec2);
			point2DCollider.centerOffset = { tmpVec2[0] ,tmpVec2[1] };

			ImGui::Checkbox("RenderFlag", &point2DCollider.renderFlag);
			p->UpdateComponent<Point2DCollider>(point2DCollider);
			if (ImGui::Button("Remove Point2DCollider"))
			{
				p->RemoveComponent<Point2DCollider>();
				LOG_INFO("Point2DCollider component removed");
			}
		}
	}
	if (p->HasComponent<Physics2D>())
	{
		if (ImGui::CollapsingHeader("Physics2D")) {
			//ImGui::Text("Physics2D");
			Physics2D physics2D = p->GetComponent<Physics2D>();
			ImGui::Checkbox("dynamicsEnabled", &e.GetComponent<Physics2D>().dynamicsEnabled);
			ImGui::InputFloat("Mass", &physics2D.mass);
			ImGui::InputFloat("inertia", &physics2D.inertia);
			ImGui::InputFloat("restitution", &physics2D.restitution);
			ImGui::InputFloat("friction", &physics2D.friction);
			ImGui::InputFloat("damping", &physics2D.damping);

			tmpVec2[0] = physics2D.velocity.x;
			tmpVec2[1] = physics2D.velocity.y;
			ImGui::InputFloat2("velocity", tmpVec2);
			physics2D.velocity = { tmpVec2[0] ,tmpVec2[1] };

			tmpVec2[0] = physics2D.acceleration.x;
			tmpVec2[1] = physics2D.acceleration.y;
			ImGui::InputFloat2("acceleration", tmpVec2);
			physics2D.acceleration = { tmpVec2[0] ,tmpVec2[1] };


			tmpVec2[0] = physics2D.accumulatedForce.x;
			tmpVec2[1] = physics2D.accumulatedForce.y;
			ImGui::InputFloat2("accumulatedForce", tmpVec2);
			physics2D.accumulatedForce = { tmpVec2[0] ,tmpVec2[1] };

			ImGui::InputFloat("angularVelocity", &physics2D.angularVelocity);
			ImGui::InputFloat("angularTorque", &physics2D.angularTorque);
			ImGui::Checkbox("Physics RenderFlag", &physics2D.renderFlag);


			p->UpdateComponent<Physics2D>(physics2D);
			if (ImGui::Button("Remove Physics2D"))
			{
				p->RemoveComponent<Physics2D>();
				LOG_INFO("Physics2D component removed");
			}
		}
	}
	if (p->HasComponent<Audio>())
	{
		if (ImGui::CollapsingHeader("Audio")) {
			//ImGui::Text("Audio");
			Audio audio = p->GetComponent<Audio>();
			ImGui::InputText("Addsound", &audio.sound.path);
			ImGui::Checkbox("Pause", &audio.sound.isPaused);
			//ImGui::Checkbox("Click to Play", &audio.sound.toPlay);
			if (ImGui::Button("Play", ImVec2(50, 10)))
			{
				audioManager->PlayAnySound(audio.sound.path, 17);
			}
			//ImGui::Checkbox("isLoop", &audio.sound.isLoop);
			p->UpdateComponent<Audio>(audio);
			if (ImGui::Button("Remove Audio"))
			{
				p->RemoveComponent<Audio>();
				LOG_INFO("Audio component removed");
			}
		}
	}
	if (p->HasComponent<Text>())
	{
		if (ImGui::CollapsingHeader("Text")) {
			//ImGui::Text("Text");
			Text text = p->GetComponent<Text>();
			ImGui::InputText("Addstrtext", &text.text);
			ImGui::InputText("Addfont", &text.fontFile);

			tmpVec2[0] = text.offset.x;
			tmpVec2[1] = text.offset.y;
			ImGui::DragFloat2("Text Pos", tmpVec2);
			text.offset = Math::Vec2(tmpVec2[0], tmpVec2[1]);

			ImGui::DragFloat("Text Scale", &text.scale);

			tmpVec4[0] = text.color.r / 255.f;
			tmpVec4[1] = text.color.g / 255.f;
			tmpVec4[2] = text.color.b / 255.f;
			tmpVec4[3] = text.color.a / 255.f;
			ImGui::ColorEdit4("Text Color", tmpVec4);
			text.color.r = (GLubyte)(tmpVec4[0] * 255);
			text.color.g = (GLubyte)(tmpVec4[1] * 255);
			text.color.b = (GLubyte)(tmpVec4[2] * 255);
			text.color.a = (GLubyte)(tmpVec4[3] * 255);
			p->UpdateComponent<Text>(text);

			if (ImGui::Button("Remove Text"))
			{
				e.RemoveComponent<Text>();
				LOG_INFO("Text component removed");
			}
		}
	}
	if (p->HasComponent<AI>())
	{
		if (ImGui::CollapsingHeader("AI")) {
			AI ai = p->GetComponent<AI>();
			//static const char* colorChange[]{ "None","Smoothy","Traffic Light" };
			int colorChangeID = ai.colorChange;
			ImGui::Combo("Select Color Change", &colorChangeID, colorChange, IM_ARRAYSIZE(colorChange));
			ai.colorChange = colorChangeID;

			//static const char* movement[]{ "None","UP-Down","Left-Right", "Swing", "Circle" };
			int movementID = ai.movement;
			ImGui::Combo("Select Movement", &movementID, movement, IM_ARRAYSIZE(movement));
			ai.movement = movementID;

			if (ai.movement)
			{
				float speed = ai.speed;
				ImGui::SliderFloat("Speed", &speed, 0.f, 15.f);
				ai.speed = speed;
				if (ai.movement < 4)
				{
					float range = ai.range;
					ImGui::SliderFloat("Range", &range, 0.f, 400.f);
					ai.range = range;
				}
			}
			p->UpdateComponent<AI>(ai);
			if (ImGui::Button("Remove AI"))
			{
				p->RemoveComponent<AI>();
				LOG_INFO("AI component removed");
			}
		}
	}
	if (p->HasComponent<Script>())
	{
		if (ImGui::CollapsingHeader("Script")) {
			ImGui::InputText("Add script file name", &e.GetComponent<Script>().name);
			if (ImGui::Button("Remove Script"))
			{
				e.RemoveComponent<Script>();
				LOG_INFO("Script component removed");
			}
		}
	}
	if (p->HasComponent<Dialogue>())
	{
		if (ImGui::CollapsingHeader("Dialogue"))
		{
			std::string dia = "Dialogue WIP";
			//ImGui:Text(dia.c_str());
			if (ImGui::Button("Remove Dialogue"))
			{
				e.RemoveComponent<Dialogue>();
				LOG_INFO("Dialogue component removed");
			}
		}
	}

}