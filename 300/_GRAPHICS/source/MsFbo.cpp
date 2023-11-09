#include <MsFbo.hpp>

void GFX::MsFBO::Create(int width, int height, bool resize)
{
	mWidth = width;
	mHeight = height;

	if (!resize)
	{
		// Create and bind framebuffer
		glGenFramebuffers(1, &mID);
		glBindFramebuffer(GL_FRAMEBUFFER, mID);
	}

	// Creating Game Attachment
	glGenTextures(1, &mColorAttachment);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mColorAttachment);
	// Specifying texture size
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, mSamples, GL_RGBA32F, width, height, GL_TRUE);
	float maxAnisotropy{};
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
	glTexParameterf(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);	// Always set the maximum available Anisotropy
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

	// Creating Entity ID Attachment
	glGenTextures(1, &mEntityIDAttachment);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mEntityIDAttachment);
	// Specifying size
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, mSamples, GL_R32UI, width, height, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

	// Creating BrightColors Attachment
	glGenTextures(1, &mBrightColorsAttachment);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mBrightColorsAttachment);
	// Specifying size
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, mSamples, GL_RGBA32F, width, height, GL_TRUE);
	glTexParameterf(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2.f);	// Anisotropy
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

	// Attach all Attachments to currently bound framebuffer
	glNamedFramebufferTexture(mID, GL_COLOR_ATTACHMENT0, mColorAttachment, 0);			// 0
	glNamedFramebufferTexture(mID, GL_COLOR_ATTACHMENT1, mEntityIDAttachment, 0);		// 1
	glNamedFramebufferTexture(mID, GL_COLOR_ATTACHMENT2, mBrightColorsAttachment, 0);	// 2

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

	// Clear Default color attachment
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glClearColor(.2f, .2f, .2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Clear Entity ID buffer
	glDrawBuffer(GL_COLOR_ATTACHMENT1);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Clear Bright Colors Attachment
	glDrawBuffer(GL_COLOR_ATTACHMENT2);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Set all attachments for output
	GLuint allAttachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, allAttachments);

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

	Create(width, height, true);
}
