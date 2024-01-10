#include <Graphics/GraphicsSystemDefinitions.h>
#include <Graphics/GraphicsSystem.h>


void InitUIMeshes()
{
	systemManager->mGraphicsSystem->m_HealthbarMesh.Setup2DImageMesh();
	systemManager->mGraphicsSystem->m_Image2DMesh.Setup2DImageMesh();
	systemManager->mGraphicsSystem->m_PortalMesh.Setup2DImageMesh();
	systemManager->mGraphicsSystem->m_ParticleMesh.Setup2DImageMesh();

	for (int i{}; i < 32; ++i)
	{
		systemManager->mGraphicsSystem->m_Textures.emplace_back(i);
	}
}

void InitFramebuffers()
{
	int width = systemManager->mGraphicsSystem->m_Width;
	int height = systemManager->mGraphicsSystem->m_Height;
	bool editormode = systemManager->mGraphicsSystem->m_EditorMode;

	systemManager->mGraphicsSystem->m_Fbo.Create(width, height, editormode);
	systemManager->mGraphicsSystem->m_GameFbo.Create(width, height, editormode);
	systemManager->mGraphicsSystem->m_MultisampleFBO.Create(width, height);
	systemManager->mGraphicsSystem->m_IntermediateFBO.Create(width, height);
	systemManager->mGraphicsSystem->m_PingPongFbo.Create(width, height);
	systemManager->mGraphicsSystem->m_PhysBloomRenderer.Init(width, height);

	// Input
	glBindImageTexture(2, systemManager->mGraphicsSystem->m_IntermediateFBO.GetBrightColorsAttachment(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	glBindImageTexture(3, systemManager->mGraphicsSystem->m_IntermediateFBO.GetFragPosAttachment(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	glBindImageTexture(4, systemManager->mGraphicsSystem->m_IntermediateFBO.GetNormalAttachment(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	glBindImageTexture(5, systemManager->mGraphicsSystem->m_IntermediateFBO.GetAlbedoSpecAttachment(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	glBindImageTexture(6, systemManager->mGraphicsSystem->m_IntermediateFBO.GetEmissionAttachment(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
}