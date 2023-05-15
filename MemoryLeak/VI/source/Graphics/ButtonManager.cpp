/*!*****************************************************************************
\file ButtonManager.cpp
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 20-11-2022
\brief
This file contains function definitions for a TransformManager system to
manipulate its components.
*******************************************************************************/
#include "ButtonManager.h"
#include "Input.h"
#include "VertexFetcher.h"
#include <ECSManager.h>

/*!*****************************************************************************
\brief
Initializes the ButtonManager.

\param int* _windowWidth
The window width pointer.

\param int* _windowHeight
The window height pointer.
*******************************************************************************/
void ButtonManager::Init(int* _windowWidth, int* _windowHeight)
{
	mWindowWidth = _windowWidth;
	mWindowHeight = _windowHeight;
	mInitialWidth = *_windowWidth;
	mInitialHeight = *_windowHeight;
}

/*!*****************************************************************************
\brief
Updates the state of all buttons.
*******************************************************************************/
void ButtonManager::Update()
{
	for (const Entity& e : mEntities)
	{
		bool prev;
		if (!e.ShouldRun()) continue;
		prev = e.GetComponent<Button>().isHover;
		e.GetComponent<Button>().isHover = CheckHover(e);
		if (e.HasComponent<MovementAI>() && e.GetComponent<MovementAI>().moveOnHover)
		{
			if (!prev && e.GetComponent<Button>().isHover)
			{
				movementAIManager->SetNextStep(e, 1);
				e.GetComponent<MovementAI>().state = 0;
				e.GetComponent<MovementAI>().run = true;
			}
			else if (prev && !e.GetComponent<Button>().isHover)
			{
				movementAIManager->SetNextStep(e, 0);
				e.GetComponent<MovementAI>().state = 0;
				e.GetComponent<MovementAI>().run = true;
			}
		}
		if (e.HasComponent<ColorAI>() && e.GetComponent<ColorAI>().changeOnHover)
		{
			if (!prev && e.GetComponent<Button>().isHover)
			{
				colorAIManager->SetNextStep(e, 1);
				e.GetComponent<ColorAI>().run = true;
			}
			else if (prev && !e.GetComponent<Button>().isHover)
			{
				colorAIManager->SetNextStep(e, 0);
				e.GetComponent<ColorAI>().run = true;
			}
		}
		e.GetComponent<Button>().isClick = CheckClick(e);
		e.GetComponent<Button>().activated = CheckActivate(e);
	}
}

/*!*****************************************************************************
\brief
Check if mouse is hovering over the button in the entity.

\param const Entity& _e
The entity to check for.
*******************************************************************************/
bool ButtonManager::CheckHover(const Entity& _e)
{
	Math::Vec2 cursorPos = Math::Vec2(Input::CursorPos().x, -Input::CursorPos().y) +
		Math::Vec2(-*mWindowWidth / 2.f, *mWindowHeight / 2.f);
#ifdef _EDITOR
	cursorPos = editorManager->GetEditorWorldMousePos();
#else
	cursorPos = Math::Vec2(Input::CursorPos().x, -Input::CursorPos().y) + Math::Vec2(-*mWindowWidth / 2.f, *mWindowHeight / 2.f);
	cursorPos.x = cursorPos.x / (*mWindowWidth / 2) * (mInitialWidth / 2);
	cursorPos.y = cursorPos.y / (*mWindowHeight / 2) * (mInitialHeight / 2);
#endif

	Transform xform = _e.GetComponent<Transform>();
	if (!(cursorPos.x <= xform.translation.x + 0.5f * xform.scale.x)) return false;
	if (!(cursorPos.x >= xform.translation.x - 0.5f * xform.scale.x)) return false;
	if (!(cursorPos.y <= xform.translation.y + 0.5f * xform.scale.y)) return false;
	if (!(cursorPos.y >= xform.translation.y - 0.5f * xform.scale.y)) return false;

	return true;
}

/*!*****************************************************************************
\brief
Check if mouse clicked the button in the entity.

\param const Entity& _e
The entity to check for.
*******************************************************************************/
bool ButtonManager::CheckClick(const Entity& _e)
{
	if (CheckHover(_e) && Input::CheckKey(E_STATE::PRESS, E_KEY::M_BUTTON_L))
		return true;
	return false;
}

/*!*****************************************************************************
\brief
Check if mouse activated the button in the entity.

\param const Entity& _e
The entity to check for.
*******************************************************************************/
bool ButtonManager::CheckActivate(const Entity& _e)
{
	if (CheckHover(_e) && Input::CheckKey(E_STATE::RELEASE, E_KEY::M_BUTTON_L))
	{
		if (_e.HasComponent<Audio>())
		{
			//_e.GetComponent<Audio>().sound.toPlay = true;
			audioManager->PlaySound(_e);
		}
		return true;
	}
	return false;
}

void ButtonManager::ResetAllButtons()
{
	for (auto& e : mEntities) {
		e.GetComponent<Button>(safe).activated = false;
	}
}

bool ButtonManager::AllNotHover()
{
	for (auto& e : mEntities)
		if (e.ShouldRun())
			if (e.HasComponent<Button>())
				if (e.GetComponent<Button>().isHover)
					return false;
	return true;
}

bool ButtonManager::AllNotClick()
{
	for (auto& e : mEntities)
		if (e.ShouldRun()) 
			if (e.HasComponent<Button>())
				if (e.GetComponent<Button>().isClick)
					return false;
	return true;
}