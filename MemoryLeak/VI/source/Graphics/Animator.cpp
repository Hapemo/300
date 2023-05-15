/*!*****************************************************************************
\file Animator.cpp
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 20-09-2022
\brief
This file contains function definitions for an Animator system that operates on
Entities with the Sprite and Animation Component.
*******************************************************************************/
#include "Graphics/Animator.h"

/*!*****************************************************************************
\brief
Animates a single Entity with Components Sprite and Animation.

\return
None.
*******************************************************************************/
void Animator::Animate()
{
	for (const Entity& e : mEntities)
		Animate(e);
}

/*!*****************************************************************************
\brief
Animates all Entities with Components Sprite and Animation.

\param const Entity& e
The Entity to be animated.

\return
None.
*******************************************************************************/
void Animator::Animate(const Entity& _e)
{
	if (!_e.GetComponent<General>().isActive) return;
	if (!_e.HasComponent<Sprite>()) return;
	if (!_e.ShouldRun()) return;
	if (!_e.HasComponent<Animation>()) return;
	if (!_e.GetComponent<Animation>().sheets.size()) return;
	if (_e.GetComponent<Animation>().currentImageIndex > _e.GetComponent<Animation>().sheets.size() - 1) return;

	Animation animation = _e.GetComponent<Animation>();

	_e.GetComponent<Sprite>().texture = animation.sheets[animation.currentImageIndex].sheet;

	if (!_e.HasComponent<SheetAnimation>()) return;
	_e.GetComponent<SheetAnimation>().frameCount = animation.sheets[animation.currentImageIndex].frameCount;
	_e.GetComponent<SheetAnimation>().timePerFrame = animation.sheets[animation.currentImageIndex].timePerFrame;
}

/*!*****************************************************************************
\brief
Adds an image to the Entity with an Animation Component. This increases the
Animation by one frame (the image added).

\param const Entity& e
The Entity whose Animation Component will have another image.

\param GLuint frame
The texture ID of the image stored in OpenGL to be added to the Animation
Component.

\return
None.
*******************************************************************************/
void Animator::AddImages(Entity _e, const SpriteSheet& _sheet)
{
	_e.GetComponent<Animation>().sheets.push_back(_sheet);
}

/*!*****************************************************************************
\brief
Adds a series of images to the Animation Component. This increases the Animation
by

\param const Entity& e
The Entity whose Animation Component will have a series of images added.

\param const std::vector<GLuint>& frames
A vector of texture ID of images stored in OpenGL to be added to the Animation
component

\return
None.
*******************************************************************************/
void Animator::AddImages(Entity _e, const std::vector<SpriteSheet>& _sheet)
{
	_e.GetComponent<Animation>().sheets.insert(_e.GetComponent<Animation>().sheets.begin(), _sheet.begin(), _sheet.end());
}
