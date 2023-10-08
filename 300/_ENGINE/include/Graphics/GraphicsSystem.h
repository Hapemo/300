/*!*************************************************************************
****
\file			GraphicsSystem.h
\author			Richmond
\par DP email:	r.choo@digipen.edu
\date			28/9/23
\brief
	The declarations for the graphics system that works within the ECS

****************************************************************************
***/

#pragma once
#ifndef _GRAPHICS_SYSTEM_H
#define _GRAPHICS_SYSTEM_H

#include <pch.h>
#include <ECS/ECS_Systems.h>
#include <ECS/ECS_Components.h>
#include <ECS/ECS.h>

#include <DebugRenderer.hpp>
#include <Camera.hpp>
#include <Fbo.hpp>
#include <Animator.hpp>

/***************************************************************************/
/*!
\brief
	the types of camera present/
	this is used to choose which camera we should update
*/
/**************************************************************************/
enum class CAMERA_TYPE
{
	CAMERA_TYPE_GAME,
	CAMERA_TYPE_EDITOR,
	CAMERA_TYPE_ALL
};


/***************************************************************************/
/*!
\brief
	The graphics system class declarations
*/
/**************************************************************************/
class GraphicsSystem
{
public:

	/***************************************************************************/
	/*!
	\brief
		the default constructor for the graphics system
	*/
	/**************************************************************************/
	GraphicsSystem() = default;

	/***************************************************************************/
	/*!
	\brief
		initializes the graphics system.
		initializes the fbos, cameras, window dimensions
	*/
	/**************************************************************************/
	void Init();

	/***************************************************************************/
	/*!
	\brief
		Updates the graphics system, the camera, and the renderer.
		pushes the calculated data into the renderer for instanced rendering
	*/
	/**************************************************************************/
	void Update(float dt);

	/***************************************************************************/
	/*!
	\brief
		the draw function isolated to the game's framebuffer
	*/
	/**************************************************************************/
	void GameDraw(float dt);

	/***************************************************************************/
	/*!
	\brief
		the draw function isolated to the editor's framebuffer
	*/
	/**************************************************************************/
	void EditorDraw(float dt);

	/***************************************************************************/
	/*!
	\brief
		exit the graphics system. unallocates resources
	*/
	/**************************************************************************/
	void Exit();

	// -- Mesh --
	/***************************************************************************/
	/*!
	\brief
		Adds an instance of the mesh to be drawn. Populates the LTW matrix, and
		the entity ID for object identification.
		This function is essential for instanced rendering
	*/
	/**************************************************************************/
	void AddInstance(GFX::Mesh& mesh, Transform transform, unsigned entityID = 0xFFFFFFFF);		// Adds an instance of a mesh to be drawn
	void AddInstance(GFX::Mesh& mesh, mat4 transform, unsigned entityID = 0xFFFFFFFF);	// Adds an instance of a mesh to be drawn

	// -- FBO --
	/***************************************************************************/
	/*!
	\brief
		Getters
	*/
	/**************************************************************************/
	vec3 GetCameraPosition(CAMERA_TYPE type);
	vec3 GetCameraTarget(CAMERA_TYPE type);
	vec3 GetCameraDirection(CAMERA_TYPE type);			// Direction vector of the camera (Target - position)
	GFX::DebugRenderer& getDebugRenderer() { 
		return m_Renderer; 
	}
	unsigned int GetGameAttachment() {
		return m_GameFbo.GetColorAttachment(); 
	}
	unsigned int GetEditorAttachment() {
		return m_Fbo.GetColorAttachment(); 
	}
	unsigned int GetEntityID(float x, float y) {
		return m_Fbo.ReadEntityID(x, y); 
	}

	// -- Camera Functions --
	/***************************************************************************/
	/*!
	\brief
		Setters
	*/
	/**************************************************************************/
	void SetCameraPosition(CAMERA_TYPE type, vec3 position);
	void SetCameraTarget(CAMERA_TYPE type, vec3 position);
	void SetCameraProjection(CAMERA_TYPE type, float fovDegree, ivec2 size, float nearZ, float farZ);
	void SetCameraSize(CAMERA_TYPE type, ivec2 size);

	/***************************************************************************/
	/*!
	\brief
		Updates the camera according to the enum and dt
	*/
	/**************************************************************************/
	void UpdateCamera(CAMERA_TYPE type, const float&);

	/***************************************************************************/
	/*!
	\brief
		Renders all instances of a given mesh
	*/
	/**************************************************************************/
	void DrawAll(GFX::Mesh& mesh);		// Renders all instances of a given mesh

	/***************************************************************************/
	/*!
	\brief
		Helper functions
	*/
	/**************************************************************************/
	void PrintMat4(const glm::mat4& input);
	void PauseGlobalAnimation();
	void UnpauseGlobalAnimation();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//		MEMBER VARIABLES MEMBER VARIABLES MEMBER VARIABLES MEMBER VARIABLES MEMBER VARIABLES MEMBER VARIABLES
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// -- Renderer --
	GFX::DebugRenderer m_Renderer;		// isolated to debug draws
	GFX::FBO m_Fbo;						// Editor Scene
	GFX::FBO m_GameFbo;					// Game Scene

	// -- SSBO -- 
	unsigned m_Ssbo;
	void SetupShaderStorageBuffer();
	void ShaderStorageBufferSubData(size_t dataSize, const void* data);
	std::vector<mat4> finalBoneMatrices;

	// -- Window --
	GFX::Window* m_Window;
	int m_Width;
	int m_Height;

	// -- Camera --
	GFX::Camera m_EditorCamera;
	CAMERA_TYPE m_CameraControl;

	// -- Textures --
	std::vector<int> m_Textures;

	// -- Flags --
	int		m_DebugDrawing{ 1 };			// debug drawing 
	bool	m_EditorMode;
	bool	m_EnableGlobalAnimations{ 1 };
	bool	m_HasLight{ false };
};

#endif