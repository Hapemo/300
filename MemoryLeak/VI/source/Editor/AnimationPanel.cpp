 /*!*****************************************************************************
/*!*****************************************************************************
\file AnimationPanel.cpp
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Group: Memory Leak Studios
\date 20-10-2022
\brief
This file contains function definations for a AnimationPanel Editor system that modifies
and Animation 
*******************************************************************************/
#include "AnimationPanel.h"
#include <ECSManager.h>

/*!*****************************************************************************
\brief
	Initializes the Animation Panel editor

\return
None.
*******************************************************************************/
void AnimationPanel::Init()
{
	viewportSize = { 0,0 };
	isViewportPaused = false;
}
/*!*****************************************************************************
\brief
	Updates the Animation Panel editor

\return
None.
*******************************************************************************/
void AnimationPanel::Update()
{
	if (ImGui::Begin("Animation Editor"))
	{
		SetViewportAspectRatio();
		renderUI();

		if (ImGui::IsWindowFocused())
			isAnimatorEditor = true;
		else
			isAnimatorEditor = false;
		
		if (selectedEntity != nullptr)
		{
			animator->Animate();
			Entity e = *selectedEntity;
			if (e.HasComponent<Sprite>())
			{
				float size = ImGui::GetWindowSize().y / 3.f;
				static int frame = 1;
				float ratio = e.GetComponent<Transform>().scale.x / e.GetComponent<Transform>().scale.y;
				ImVec2 imageSize = { size * ratio * frame,size };
				GLuint animation_texture = spriteManager->GetTexture(e);
				ImVec2 viewSize = { size * 16.f/9.f,size };
				
				if (e.HasComponent<SheetAnimation>())
				{
					frame = e.GetComponent<SheetAnimation>().frameCount;
					renderManager->GetAnimatorCamera().SetZoom(e.GetComponent<Transform>().scale.y / *mWindowHeight);
					renderManager->GetAnimatorCamera().SetPos(e.GetComponent<Transform>().translation);
					textureImage = (void*)(intptr_t)renderManager->GetAnimatorFBO();
					ImGui::Image(textureImage, viewSize, ImVec2(0, 1), ImVec2(1, 0));
					if (ImGui::BeginDragDropTarget())
					{
						ImGui::EndDragDropTarget();
					}
					if (!isViewportPaused)
					{
						if (ImGui::IsWindowFocused())
							sheetAnimator->Animate();
						else
							isViewportPaused = true;
					}
					if (!e.HasComponent<Animation>())
					{
						ImGui::InputInt("Frame Count", &e.GetComponent<SheetAnimation>().frameCount, 1,2);
						e.GetComponent<SheetAnimation>().frameCount < 0 ? 0 : e.GetComponent<SheetAnimation>().frameCount;
					}
					ImGui::SliderInt("Frame", &e.GetComponent<SheetAnimation>().currFrameIndex, 0, e.GetComponent<SheetAnimation>().frameCount-1);
				}
				else
				{
					if (ImGui::Button("Add Animation Sheet Component", {viewSize.x/2.f, viewSize.y/2.f}))
					{
						e.AddComponent<SheetAnimation>({});
					}
				}
				if (!e.HasComponent<Animation>())
				{
					if (ImGui::Button("Add Animation Component", { viewSize.x / 2.f, viewSize.y / 2.f }))
					{
						e.AddComponent<Animation>({});
						if (e.HasComponent<Sprite>())
							animator->AddImages(e, SpriteSheet{ e.GetComponent<Sprite>(safe).texture });
					}
					if (animation_texture)
					{
						textureImage = (void*)(intptr_t)animation_texture;
						ImGui::Image(textureImage, imageSize, ImVec2(0, 1), ImVec2(1, 0));
						if (ImGui::BeginDragDropTarget())
						{
							static const wchar_t* texpath = (const wchar_t*)"";
							if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURES"))
							{
								texpath = (const wchar_t*)payload->Data;
								std::string tp = (std::string)((const char*)texpath);
								e.GetComponent<Sprite>().sprite = SPRITE::TEXTURE;
								spriteManager->SetTexture(e, tp);
							}
							ImGui::EndDragDropTarget();
						}

					}
				}
				else
				{
					ImGui::SliderInt("Sheet", &e.GetComponent<Animation>().currentImageIndex,0, (int)e.GetComponent<Animation>().sheets.size()-1);
					ImGui::InputInt("Sheet Count", &e.GetComponent<Animation>().sheets[e.GetComponent<Animation>().currentImageIndex].frameCount, 1, 2);
					e.GetComponent<Animation>().sheets[e.GetComponent<Animation>().currentImageIndex].frameCount < 0 ? 0 : e.GetComponent<Animation>().sheets[e.GetComponent<Animation>().currentImageIndex].frameCount;
					
					for (size_t i = 0; i < e.GetComponent<Animation>().sheets.size(); i++)
					{
						if(size * ratio * e.GetComponent<Animation>().sheets[i].frameCount<ImGui::GetWindowWidth())
							imageSize = { size * ratio * e.GetComponent<Animation>().sheets[i].frameCount,size};
						else
							imageSize = { ImGui::GetWindowWidth()-100.f, (ImGui::GetWindowWidth()-100.f) / ratio / e.GetComponent<Animation>().sheets[i].frameCount };
						textureImage = (void*)(intptr_t)e.GetComponent<Animation>().sheets[i].sheet;
						float sheetWidth = imageSize.x / e.GetComponent<Animation>().sheets[i].frameCount;
						ImVec2 p = ImGui::GetCursorScreenPos();
						ImGui::Image(textureImage, imageSize, ImVec2(0, 1), ImVec2(1, 0));
						for(int w =1; w< e.GetComponent<Animation>().sheets[i].frameCount ; w++)
							ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x+ sheetWidth*w, p.y), ImVec2(p.x+ sheetWidth*w, p.y + imageSize.y), IM_COL32(30*w, 0, 30 * (8 - w), 255), 1.0f);
						ImGui::SameLine();
						ImGui::Text(("Sheet " + std::to_string(i)).c_str());
					}
					//if (!isViewportPaused)
					//{
						//if (ImGui::IsWindowFocused())
							//animator->Animate();
						//else
							//isViewportPaused = true;
					//}
				}
			}
		}
		else
		{
			ImGui::Text("Select an entity to edit");
		}
		
	}
	ImGui::End();
}
/*!*****************************************************************************
\brief
	Free the Animation Panel editor

\return
None.
*******************************************************************************/
void AnimationPanel::Free()
{

}

