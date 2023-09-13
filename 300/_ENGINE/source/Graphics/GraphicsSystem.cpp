#include <Graphics/GraphicsSystem.h>


void GraphicsSystem::Init()
{
	// TODO: Create FBO, will need the width and height of the window
	//m_Fbo.Create();
}

void GraphicsSystem::Update()
{
	// TODO: Retrieve the mesh instances to be drawn

	// Prepare and bind the Framebuffer to be rendered on
	m_Fbo.PrepForDraw();

	// TODO: Render all instances of a given mesh

	// TODO: Clears all instances that have been rendered from local buffer
}

void GraphicsSystem::Exit()
{

}

void GraphicsSystem::AddInstance(GFX::Mesh& mesh, Transform transform)
{
	// Local to world transformation
	mat4 world = {
		vec4(transform.mScale.x, 0.f, 0.f, 0.f),
		vec4(0.f, transform.mScale.y, 0.f, 0.f),
		vec4(0.f, 0.f, transform.mScale.z, 0.f),
		vec4(transform.mTranslate, 1.f)
	};

	// TODO: To combine rotation transformation as well
	mesh.mLTW.push_back(world);
}

void GraphicsSystem::DrawAll(GFX::Mesh& mesh)
{
	// TODO: Activate the necessary Shader

	// TODO: Bind the required textures

	mesh.DrawAllInstances();

	// TODO: Unbind the bound textures

	// TODO: Deactivate the activated shader
}
