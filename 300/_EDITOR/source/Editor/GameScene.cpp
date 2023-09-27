#include "GameScene.h"




	void GameScene::init() 
	{

	}


	void GameScene::update() 
	{
		mWinFlag |= ImGuiWindowFlags_NoScrollbar;

		const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		ImGui::Image((ImTextureID)(intptr_t)systemManager->mGraphicsSystem->GetGameAttachment(), viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));

		winSize = { ImGui::GetWindowSize().x ,ImGui::GetWindowSize().y };

	}
