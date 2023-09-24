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

enum class CAMERA_TYPE
{
	CAMERA_TYPE_GAME,
	CAMERA_TYPE_EDITOR,
	CAMERA_TYPE_ALL
};


class GraphicsSystem
{
public:
	GraphicsSystem() = default;

	void Init();
	void Update(float dt);
	void Exit();

	// -- Mesh --
	void AddInstance(GFX::Mesh& mesh, Transform transform);		// Adds an instance of a mesh to be drawn

	// -- FBO --
	unsigned int GetGameAttachment()		{ return m_Fbo.GetGameAttachment(); }
	unsigned int GetEditorAttachment()		{ return m_Fbo.GetEditorAttachment(); }
	unsigned int GetEntityID(int x, int y)	{ return m_Fbo.ReadEntityID(x, y); }

	// -- Getter --
	GFX::DebugRenderer& getDebugRenderer() { return m_Renderer; }

	// -- Camera Functions --
	void SetCameraPosition(CAMERA_TYPE type, vec3 position);
	void SetCameraTarget(CAMERA_TYPE type, vec3 position);
	void SetCameraProjection(CAMERA_TYPE type, float fovDegree, ivec2 size, float nearZ, float farZ);
	void SetCameraSize(CAMERA_TYPE type, ivec2 size);
	void UpdateCamera(CAMERA_TYPE type, const float&);

	vec3 GetCameraPosition(CAMERA_TYPE type);
	vec3 GetCameraTarget(CAMERA_TYPE type);

	// Direction vector of the camera (Target - position)
	vec3 GetCameraDirection(CAMERA_TYPE type);

	// Helper Function
	void PrintMat4(const glm::mat4& input);

private:
	GFX::DebugRenderer m_Renderer;		// isolated to debug draws
	GFX::FBO m_Fbo;

	// -- Window --
	GFX::Window* m_Window;
	int m_Width;
	int m_Height;

	// -- Camera --
	GFX::Camera m_GameCamera;
	GFX::Camera m_EditorCamera;

	// -- Textures --
	std::vector<int> m_Textures;

	// -- Flags --
	bool m_EditorMode;
	bool m_DebugDrawing{ 1 };			// debug drawing 

	// -- Private Functions --
	void DrawAll(GFX::Mesh& mesh);		// Renders all instances of a given mesh

};

#endif