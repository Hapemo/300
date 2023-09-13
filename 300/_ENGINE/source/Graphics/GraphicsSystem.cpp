#include <Graphics/GraphicsSystem.h>
#include <ECS/ECS_Systems.h>


void GraphicsSystem::Init()
{
	// Get Window Handle
	m_Window = systemManager->GetWindow();
	m_Width = m_Window->size().x;
	m_Height = m_Window->size().y;

	m_EditorMode = systemManager->IsEditor();

	// Create FBO, with the width and height of the window
	m_Fbo.Create(m_Width, m_Height, m_EditorMode);
}

void GraphicsSystem::Update(float dt)
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
	mat4 scale = glm::scale(transform.mScale);
	mat4 translate = glm::translate(transform.mTranslate);

	glm::quat quatRotate = glm::quat(transform.mRotate);	// Retrieve quaternions via euler angles
	glm::mat4 rotation = glm::toMat4(quatRotate);			// Retrieve rotation matrix via quaternions

	mat4 world = translate * rotation * scale;
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
