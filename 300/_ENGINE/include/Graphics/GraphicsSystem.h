#pragma once
#ifndef _GRAPHICS_SYSTEM_H
#define _GRAPHICS_SYSTEM_H

#include <pch.h>
#include <ECS/ECS_Components.h>
#include <ECS/ECS.h>

#include <DebugRenderer.hpp>
#include <Fbo.hpp>

class GraphicsSystem
{
public:
	GraphicsSystem() = default;

	void Init();
	void Update();
	void Exit();

	// -- Mesh --
	void AddInstance(GFX::Mesh& mesh, Transform transform);		// Adds an instance of a mesh to be drawn

	// -- FBO --
	unsigned int GetGameAttachment()	{ return m_Fbo.GetGameAttachment(); }
	unsigned int GetEditorAttachment()	{ return m_Fbo.GetEditorAttachment(); }

private:
	GFX::DebugRenderer m_Renderer;		// isolated to debug draws
	GFX::FBO m_Fbo;

	// -- Private Functions --
	void DrawAll(GFX::Mesh& mesh);		// Renders all instances of a given mesh
};

#endif