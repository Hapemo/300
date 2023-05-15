/*!*****************************************************************************
\file SpriteManager.cpp
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 20-09-2022
\brief
This file contains function definitions for the class SpriteManager, which
operates on Entities with Sprite Component.
*******************************************************************************/
#include "Graphics/SpriteManager.h"
#include "ResourceManager.h"
#include "PerformanceVisualiser.h"
#include "Logger.h"

/*!*****************************************************************************
\brief
Default Constructor for RenderManager class.
*******************************************************************************/
SpriteManager::SpriteManager()
{
	glEnable(GL_TEXTURE_2D);
}

/*!*****************************************************************************
\brief
Initializes and stores a texture in OpenGL memory.

\param ResourceManager::TextureData& _texData
A struct encapsulating the details in a Texture object.
*******************************************************************************/
void SpriteManager::InitializeTexture(ResourceManager::TextureData& _texData)
{
	if (!_texData.data) return;
	
	//create textures in OpenGL
	glCreateTextures(GL_TEXTURE_2D, 1, &_texData.id);

	//storing texture into OpenGL memory
	//if jpeg
	if (_texData.channels == 3)
	{
		glTextureStorage2D(_texData.id, 1, GL_RGB8, _texData.width, _texData.height);
		glTextureSubImage2D(_texData.id, 0, 0, 0, _texData.width, _texData.height, 
			GL_RGB, GL_UNSIGNED_BYTE, _texData.data);
	}
	//if png
	else if (_texData.channels == 4)
	{
		glTextureStorage2D(_texData.id, 1, GL_RGBA8, _texData.width, _texData.height);
		glTextureSubImage2D(_texData.id, 0, 0, 0, _texData.width, _texData.height, 
			GL_RGBA, GL_UNSIGNED_BYTE, _texData.data);
	}

	UNLOAD_ASSET(_texData.data);
}

/*!*****************************************************************************
\brief
Free all textures in OpenGL memory.
*******************************************************************************/
void SpriteManager::FreeTextures()
{
	for (GLuint* i : GET_TEXTURE_IDS())
		glDeleteTextures(1, i);
}

//------------------------------------------------------------------------------
// Getter and Setters
//------------------------------------------------------------------------------
void SpriteManager::SetColor(const Entity& e, const Color& clr)
{
	if (!e.HasComponent<Sprite>()) return;
	SetColor(e, clr.r, clr.g, clr.b, clr.a);
}

void SpriteManager::SetColor(const Entity& e, GLubyte r, GLubyte g, GLubyte b, GLubyte a)
{
	if (!e.HasComponent<Sprite>()) return;
	e.GetComponent<Sprite>().color = { r, g, b, a };
}

void SpriteManager::SetSprite(const Entity& e, SPRITE sprite)
{
	if (!e.HasComponent<Sprite>()) return;
	e.GetComponent<Sprite>().sprite = sprite;
}

void SpriteManager::SetTexture(const Entity& e, const std::string& texture_path)
{
	if (!e.HasComponent<Sprite>()) return;
	e.GetComponent<Sprite>().texture = GET_TEXTURE_ID(texture_path);
}
