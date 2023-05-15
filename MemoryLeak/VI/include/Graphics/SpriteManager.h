/*!*****************************************************************************
\file SpriteManager.h
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 20-09-2022
\brief
This file contains function declarations for the class SpriteManager, which
operates on Entities with Sprite Component.
*******************************************************************************/
#pragma once
#include "ECS_components.h"
#include <ECS_systems.h>
#include "ECS_items.h"
#include "pch.h"
#include "SpriteVariable.h"
#include "ResourceManager.h"

/*!*****************************************************************************
\brief
SpriteManager Class that handles the getting and setting of varibles in the
Sprite Component.
*******************************************************************************/
class SpriteManager : public System
{
public:
	/*!*****************************************************************************
	\brief
	Default Constructor for SpriteManager class.
	*******************************************************************************/
	SpriteManager();

	/*!*****************************************************************************
	\brief
	Initializes and stores a texture in OpenGL memory.

	\param ResourceManager::TextureData& texData
	A struct encapsulating the details in a Texture object.
	*******************************************************************************/
	void	InitializeTexture	(ResourceManager::TextureData& _texData);

	/*!*****************************************************************************
	\brief
	Free all textures in OpenGL memory.
	*******************************************************************************/
	void	FreeTextures();

	//------------------------------------------------------------------------------
	// Getter and Setters
	//------------------------------------------------------------------------------
	void SetColor(const Entity& _e, const Color& clr);
	void SetColor(const Entity& _e, GLubyte _r, GLubyte _g, 
					GLubyte _b, GLubyte _a = 255);
	Color GetColor(const Entity& _e) { return _e.GetComponent<Sprite>().color; }

	void SetSprite(const Entity& _e, SPRITE _sprite);
	SPRITE GetSprite(const Entity& _e) { return _e.GetComponent<Sprite>().sprite; }

	void SetTexture(const Entity& _e, const std::string& _texture_path);
	GLuint GetTexture(const Entity& _e)	{ return _e.GetComponent<Sprite>().texture; }
	
	GLuint	GetTextureID		(const std::string& _texture_path) { return GET_TEXTURE_ID(_texture_path); }
	std::string	GetTexturePath	(GLint _id) { return GET_TEXTURE_PATH(_id); }
private:
};