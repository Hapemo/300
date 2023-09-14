#pragma once
#ifndef _GRAPHICS_SYSTEM_H
#define _GRAPHICS_SYSTEM_H

#include <pch.h>
#include <ECS/ECS_Systems.h>
#include <ECS/ECS_Components.h>
#include <ECS/ECS.h>

#include <DebugRenderer.hpp>
#include <Fbo.hpp>

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

	// Getter
	GFX::DebugRenderer& getDebugRenderer() { return m_Renderer; }

private:
	GFX::DebugRenderer m_Renderer;		// isolated to debug draws
	GFX::FBO m_Fbo;

	// -- Window --
	GFX::Window* m_Window;
	int m_Width;
	int m_Height;

	// -- Flags --
	bool m_EditorMode;

	// -- Private Functions --
	void DrawAll(GFX::Mesh& mesh);		// Renders all instances of a given mesh
};

#endif