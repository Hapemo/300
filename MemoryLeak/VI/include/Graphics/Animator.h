/*!*****************************************************************************
\file Animator.h
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 20-09-2022
\brief
This file contains function declarations for an Animator system that operates on 
Entities with the Sprite and Animation Component. It uses !DIFFERENT TEXTURES! to
animate.
*******************************************************************************/
#pragma once
#include "pch.h"
#include <ECS_systems.h>
#include "ECS_items.h"
#include "ECS_components.h"
#include "Helper.h"
#include "AnimationVariable.h"

/*!*****************************************************************************
\brief
Animator class that operates on Entities containing the Sprite and Animation
Component.
*******************************************************************************/
class Animator : public System
{
public:
	/*!*****************************************************************************
	\brief
	Default constructor for Animator class.
	*******************************************************************************/
	Animator() = default;

	/*!*****************************************************************************
	\brief
	Animates all Entities with Components Sprite and Animation.
	
	\return 
	None.
	*******************************************************************************/
	void Animate();

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
	void AddImages(Entity _e, const SpriteSheet& _sheet);

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
	void AddImages(Entity _e, const std::vector<SpriteSheet>& _sheet);

	void SetCurrentImageIndex(Entity _e, int _index) { _e.GetComponent<Animation>().currentImageIndex = _index; }
	int GetCurrentImageIndex(Entity _e) { return  _e.GetComponent<Animation>().currentImageIndex; }
private:
	/*!*****************************************************************************
	\brief
	Animates a single Entity with Components Sprite and Animation.
	
	\param const Entity& e
	The Entity to be animated.

	\return
	None.
	*******************************************************************************/
	void Animate(const Entity& _e);
};

