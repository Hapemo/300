/*!*****************************************************************************
\file Fbo.cpp
\author Lee Fu Sheng Roy
\par DP email: f.lee@digipen.edu
\par Group: Pepe Production
\date 28-09-2023
\brief
FBO class implementation. Creates the framebuffer object. Consists of functions 
to handle resizing of framebuffers and object picking
*******************************************************************************/

#include "Fbo.hpp"

void GFX::FBO::Create(int width, int height, bool editorMode)
{
	mWidth = width;
	mHeight = height;
	mEditorMode = editorMode;

	// Create and bind framebuffer
	glGenFramebuffers(1, &mID);
	glBindFramebuffer(GL_FRAMEBUFFER, mID);

	// Creating Game Attachment
	glGenTextures(1, &mColorAttachment);
	glBindTexture(GL_TEXTURE_2D, mColorAttachment);
	// Specifying texture size
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// Creating Entity ID Attachment
	glGenTextures(1, &mEntityIDAttachment);
	glBindTexture(GL_TEXTURE_2D, mEntityIDAttachment);
	// Specifying size
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Attach all Attachments to currently bound framebuffer
	glNamedFramebufferTexture(mID, GL_COLOR_ATTACHMENT0, mColorAttachment, 0);			// 0
	glNamedFramebufferTexture(mID, GL_COLOR_ATTACHMENT1, mEntityIDAttachment, 0);		// 1

	// Creating renderbuffer object for depth testing
	glCreateRenderbuffers(1, &mRboID);
	glNamedRenderbufferStorage(mRboID, GL_DEPTH_COMPONENT, width, height);
	// Attaching renderbuffer object to framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRboID);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		throw std::runtime_error("Framebuffer creation failed");
	}
}

void GFX::FBO::PrepForDraw()
{
	// bind framebuffer as buffer to render to
	glBindFramebuffer(GL_FRAMEBUFFER, mID);

	glClearColor(.2f, .2f, .2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (mEditorMode)	// Need to render to both color attachments and Entity ID attachment
	{
		// Clear Entity ID buffer
		glDrawBuffer(GL_COLOR_ATTACHMENT1);
		glClearColor(0.f, 0.f, 0.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Game attachment, Editor Attachment, Entity ID Attachment
		// Clear Color attachments
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glClearColor(0.2f, 0.2f, 0.2f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Set all attachments for output
		GLuint allAttachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, allAttachments);
	}
	// Renders to Game Attachment by default

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

unsigned int GFX::FBO::ReadEntityID(float posX, float posY)
{
	// Map local to framebuffer space
	int mapped_x = int(posX * mWidth);
	int mapped_y = int(mHeight - posY * mHeight);
	if (mapped_x >= mWidth || mapped_x < 0 || mapped_y >= mHeight || mapped_y < 0)
		return 0xFFFFFFFF;

	glBindFramebuffer(GL_FRAMEBUFFER, mID);		// Bind FBO to be read from

	glReadBuffer(GL_COLOR_ATTACHMENT1);			// Read from entity attachment
	unsigned eid{};
	glReadPixels(mapped_x, mapped_y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &eid);	// Retrieve pixel data

	glBindFramebuffer(GL_FRAMEBUFFER, 0);		// Unbind FBO

	return eid;
}

void GFX::FBO::Resize(int width, int height)
{
	mWidth = width;
	mHeight = height;

	// Bind Color Attachment to be resized
	glBindTexture(GL_TEXTURE_2D, mColorAttachment);
	// Specifying new attachment size
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	// Unbind 
	glBindTexture(GL_TEXTURE_2D, 0);

	// Bind Entity ID Attachment to be resized
	glBindTexture(GL_TEXTURE_2D, mEntityIDAttachment);
	// Specifying new attachment size
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);

	// Unbind 
	glBindTexture(GL_TEXTURE_2D, 0);

	glNamedRenderbufferStorage(mRboID, GL_DEPTH_COMPONENT, width, height);
}
