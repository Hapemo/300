#include <MsFbo.hpp>

void GFX::MsFBO::Create(int width, int height, bool resize)
{
	// Store size
	mWidth = width;
	mHeight = height;

	if (!resize)
	{
		// Create and bind framebuffer
		glGenFramebuffers(1, &mID);
		glBindFramebuffer(GL_FRAMEBUFFER, mID);
	}

	// Creating Game Attachment
	CreateAttachment(mColorAttachment, GL_RGBA32F);
	float maxAnisotropy{};
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
	glTexParameterf(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);	// Always set the maximum available Anisotropy

	// Creating Entity ID Attachment
	CreateAttachment(mEntityIDAttachment, GL_R32UI);

	// Creating BrightColors Attachment
	CreateAttachment(mBrightColorsAttachment, GL_RGBA32F);
	glTexParameterf(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);	// Anisotropy

	// Creating G-buffer Fragment Position Attachment
	CreateAttachment(mGFragPosAttachment, GL_RGBA32F);
	glTexParameterf(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);	// Anisotropy

	// Creating G-buffer Normal Attachment
	CreateAttachment(mGNormalAttachment, GL_RGBA32F);
	glTexParameterf(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);	// Anisotropy

	// Creating G-buffer Albedo-Specular Attachment
	CreateAttachment(mGAlbedoSpecAttachment, GL_RGBA32F);
	glTexParameterf(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);	// Anisotropy

	// Creating G-buffer Emission Attachment
	CreateAttachment(mGEmissionAttachment, GL_RGBA32F);
	glTexParameterf(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);	// Anisotropy

	// Attach all Attachments to currently bound framebuffer
	glNamedFramebufferTexture(mID, GL_COLOR_ATTACHMENT0, mColorAttachment, 0);			// 0
	glNamedFramebufferTexture(mID, GL_COLOR_ATTACHMENT1, mEntityIDAttachment, 0);		// 1
	glNamedFramebufferTexture(mID, GL_COLOR_ATTACHMENT2, mBrightColorsAttachment, 0);	// 2
	glNamedFramebufferTexture(mID, GL_COLOR_ATTACHMENT3, mGFragPosAttachment, 0);		// 3
	glNamedFramebufferTexture(mID, GL_COLOR_ATTACHMENT4, mGNormalAttachment, 0);		// 4
	glNamedFramebufferTexture(mID, GL_COLOR_ATTACHMENT5, mGAlbedoSpecAttachment, 0);	// 5
	glNamedFramebufferTexture(mID, GL_COLOR_ATTACHMENT6, mGEmissionAttachment, 0);		// 6

	// Creating renderbuffer object for depth testing
	glCreateRenderbuffers(1, &mRboID);
	glNamedRenderbufferStorageMultisample(mRboID, mSamples, GL_DEPTH_COMPONENT, width, height);

	// Attaching renderbuffer object to framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRboID);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		throw std::runtime_error("Framebuffer creation failed");
	}
}

void GFX::MsFBO::PrepForDraw()
{
	// bind framebuffer as buffer to render to
	glBindFramebuffer(GL_FRAMEBUFFER, mID);

	// Set all attachments for output
	GLuint allAttachments[6] = { 
		GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, 
		GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5 
	};
	glDrawBuffers(6, allAttachments);

	// Clear Default color attachment
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GFX::MsFBO::BlitFramebuffer(unsigned int destFBO)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destFBO);

	for (int i{}; i < 3; ++i)
	{
		glReadBuffer(GL_COLOR_ATTACHMENT0 + i);
		glDrawBuffer(GL_COLOR_ATTACHMENT0 + i);
		glBlitFramebuffer(0, 0, mWidth, mHeight, 0, 0, mWidth, mHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

	glReadBuffer(GL_NONE);
	glDrawBuffer(GL_NONE);
}

void GFX::MsFBO::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mID);
}

void GFX::MsFBO::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GFX::MsFBO::Resize(int width, int height)
{
	// Delete current buffer and attachment
	glDeleteRenderbuffers(1, &mRboID);
	glDeleteTextures(1, &mColorAttachment);
	glDeleteTextures(1, &mEntityIDAttachment);
	glDeleteTextures(1, &mBrightColorsAttachment);
	glDeleteTextures(1, &mGFragPosAttachment);
	glDeleteTextures(1, &mGNormalAttachment);
	glDeleteTextures(1, &mGAlbedoSpecAttachment);
	glDeleteTextures(1, &mGEmissionAttachment);

	Create(width, height, true);
}

void GFX::MsFBO::CreateAttachment(unsigned& attachment, GLenum internalFormat)
{
	// Creating Game Attachment
	glGenTextures(1, &attachment);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, attachment);
	// Specifying texture size
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, mSamples, internalFormat, mWidth, mHeight, GL_TRUE);
}
