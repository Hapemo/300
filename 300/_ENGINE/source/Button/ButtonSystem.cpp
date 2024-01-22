#include <Button/ButtonSystem.h>

void ButtonSystem::Init()
{
	window = systemManager->GetWindow();
}

void ButtonSystem::Update()
{
	// Get Mouse Position
	double mouseX{}, mouseY{};

	if (systemManager->IsEditor())
	{
		mouseX = imguiButtonX;
		mouseY = imguiButtonY;
	}
	else {
		window->GetCursorPos(&mouseX, &mouseY);
		mouseX /= window->GetScreenWidth();
		mouseY /= window->GetScreenHeight();
	}
	mouseX *= 2.f;
	mouseX -= 1.f;
	mouseY *= -2.f;
	mouseY += 1.f;

	// Map the Mouse Position (top left origin[0, 1] to center origin)
	// Update all Button
	auto entities = systemManager->ecs->GetEntitiesWith<Button>();
	for (Entity ent : entities)
	{
		Button &button = ent.GetComponent<Button>();

		button.mIsHover = CheckHover(ent, { mouseX, mouseY });
		button.mIsClick = CheckClick(ent, { mouseX, mouseY });
		button.mActivated = CheckActivate(ent, { mouseX, mouseY });
	}
}

void ButtonSystem::Reset()
{
	auto entities = systemManager->ecs->GetEntitiesWith<Button>();
	for (Entity ent : entities)
	{
		Button &button = ent.GetComponent<Button>();

		button.mIsHover = false;
		button.mIsClick = false;
		button.mActivated = false;
	}
}

bool ButtonSystem::CheckHover(Entity e, vec2 mousePos)
{
	// Get Transform of button
	Transform buttonTransform = e.GetComponent<Transform>();
	if (mousePos.x < -0.7f) {
		mousePos.x = mousePos.x;
	}
	float scaleX = 0.5f * std::fabsf(buttonTransform.mScale.x);
	float scaleY = 0.5f * std::fabsf(buttonTransform.mScale.y);

	if (mousePos.x < buttonTransform.mTranslate.x - scaleX) return false;
	if (mousePos.x > buttonTransform.mTranslate.x + scaleX) return false;	
	if (mousePos.y < buttonTransform.mTranslate.y - scaleY) return false;
	if (mousePos.y > buttonTransform.mTranslate.y + scaleY) return false;
	return true;
}

bool ButtonSystem::CheckClick(Entity e, vec2 mousePos)
{
	return (CheckHover(e, mousePos) && Input::CheckKey(E_STATE::PRESS, E_KEY::M_BUTTON_L));
}

bool ButtonSystem::CheckActivate(Entity e, vec2 mousePos)
{
	return (CheckHover(e, mousePos) && Input::CheckKey(E_STATE::RELEASE, E_KEY::M_BUTTON_L));
}
